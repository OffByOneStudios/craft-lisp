
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/abnf.hpp>
#include <tao/pegtl/contrib/changes.hpp>
#include <tao/pegtl/contrib/unescape.hpp>
#include <tao/pegtl/contrib/tracer.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

namespace json
{
	const TaggedValue TaggedValue::Null((void*)nullptr);

	using namespace tao::pegtl;
	namespace pegtl = tao::pegtl;

	//
	// Resulting data
	//

	struct Node
	{
		enum class JsonType
		{
			None,
			Value,
			Array,
			Object,
			Slot
		};

		Identifier id;
		JsonType type;
		TaggedValue value;
		Node* parent;
		std::vector<Node*> collect;

		Node(JsonType type_)
		{
			type = type_;
		}

		Node(Node* clone_this)
		{
			id = 0;
			parent = nullptr;

			type = clone_this->type;
			value = clone_this->value;

			for (auto cn : clone_this->collect)
			{
				auto nn = new Node(cn);
				if (cn->parent == clone_this)
					nn->parent = this;
				collect.push_back(nn);
			}
		}

		~Node()
		{
			for (auto n : collect)
				delete n;
		}

		std::string slotKey()
		{
			return value.value._string;
		}

		Node* slotValue()
		{
			return collect[0];
		}
	};

	//
	// Parse rules
	//

	struct ws : one< ' ', '\t', '\n', '\r' > {};

	template< typename R, typename P = ws >
	struct padr : internal::seq< R, internal::star< P > > {};

	struct begin_array : padr< one< '[' > > {};
	struct begin_object : padr< one< '{' > > {};
	struct end_array : one< ']' > {};
	struct end_object : one< '}' > {};
	struct name_separator : pad< one< ':' >, ws > {};
	struct value_separator : padr< one< ',' > > {};

	struct false_ : tao::pegtl::string<'f', 'a', 'l', 's', 'e'> {};
	struct null : tao::pegtl::string<'n', 'u', 'l', 'l'> {};
	struct true_ : tao::pegtl::string<'t', 'r', 'u', 'e'> {};

	struct digits : plus< abnf::DIGIT > {};
	struct exp : seq< one< 'e', 'E' >, opt< one< '-', '+'> >, must< digits > > {};
	struct frac : if_must< one< '.' >, digits > {};
	struct int_ : sor< one< '0' >, digits > {};
	struct number : seq< opt< one< '-' > >, int_, opt< frac >, opt< exp > > {};

	struct xdigit : abnf::HEXDIG {};
	struct unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
	struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
	struct escaped : sor< escaped_char, unicode > {};
	struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
	struct char_ : if_then_else< one< '\\' >, must< escaped >, unescaped > {};

	struct string_content : until< at< one< '"' > >, must< char_ > > {};
	struct string : seq< one< '"' >, must< string_content >, any >
	{
		using content = string_content;
	};

	struct key_content : until< at< one< '"' > >, must< char_ > > {};
	struct key : seq< one< '"' >, must< key_content >, any >
	{
		using content = key_content;
	};

	struct value;

	struct array_element;
	struct array_content : opt< list_must< array_element, value_separator > > {};
	struct array : seq< begin_array, array_content, must< end_array > >
	{
		using begin = begin_array;
		using end = end_array;
		using element = array_element;
		using content = array_content;
	};

	struct member : if_must< key, name_separator, value > {};
	struct object_content : opt< list_must< member, value_separator > > {};
	struct object : seq< begin_object, object_content, must< end_object > >
	{
		using begin = begin_object;
		using end = end_object;
		using element = member;
		using content = object_content;
	};

	struct value : padr< sor< string, number, object, array, false_, true_, null > > {};
	struct array_element : seq< value > {};

	struct text : seq< star< ws >, value > {};

	//
	// errors
	//

	template< typename Rule >
	struct errors
		: public normal< Rule >
	{
		static const std::string error_message;

		template< typename Input, typename... States >
		static void raise(const Input& in, States&&...)
		{
			throw parse_error(error_message, in);
		}
	};

	template<> const std::string errors< json::text >::error_message = "no valid JSON";

	template<> const std::string errors< json::end_array >::error_message = "incomplete array, expected ']'";
	template<> const std::string errors< json::end_object >::error_message = "incomplete object, expected '}'";
	template<> const std::string errors< json::member >::error_message = "expected member";
	template<> const std::string errors< json::name_separator >::error_message = "expected ':'";
	template<> const std::string errors< json::array_element >::error_message = "expected value";
	template<> const std::string errors< json::value >::error_message = "expected value";

	template<> const std::string errors< json::digits >::error_message = "expected at least one digit";
	template<> const std::string errors< json::xdigit >::error_message = "incomplete universal character name";
	template<> const std::string errors< json::escaped >::error_message = "unknown escape sequence";
	template<> const std::string errors< json::char_ >::error_message = "invalid character in string";
	template<> const std::string errors< json::string::content >::error_message = "unterminated string";
	template<> const std::string errors< json::key::content >::error_message = "unterminated key";

	template<> const std::string errors< tao::pegtl::eof >::error_message = "unexpected character after JSON value";

	//
	// States and Actions
	//

	struct result_state
	{
		result_state()
		{
			result = new Node(Node::JsonType::None);
		}
		result_state(const result_state&) = delete;
		result_state(result_state&&) = delete;

		~result_state() = default;

		void operator=(const result_state&) = delete;
		void operator=(result_state&&) = delete;

		Node* result;
	};

	// Unescaped strings

	struct unescape_state_base
	{
		unescape_state_base() = default;

		unescape_state_base(const unescape_state_base&) = delete;
		void operator=(const unescape_state_base&) = delete;

		std::string unescaped;
	};

	// Action class for parsing literal strings, uses the PEGTL unescape utilities, cf. unescape.cpp.

	template< typename Rule, template< typename... > class Base = nothing >
	struct unescape_action : Base< Rule >
	{
	};

	// clang-format off
	template<> struct unescape_action< json::unicode > : unescape::unescape_j {};
	template<> struct unescape_action< json::escaped_char > : unescape::unescape_c< json::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
	template<> struct unescape_action< json::unescaped > : unescape::append_all {};
	// clang-format on

	// Action class for the simple cases...

	template< typename Rule >
	struct value_action
		: unescape_action< Rule >
	{
	};

	struct string_state
		: public unescape_state_base
	{
		void success(result_state& result)
		{
			result.result->type = Node::JsonType::Value;
			result.result->value = TaggedValue(std::move(unescaped));
		}
	};

	template<>
	struct value_action< json::null >
	{
		template<typename Input>
		static void apply(Input const&, result_state& result)
		{
			result.result->type = Node::JsonType::Value;
			result.result->value = TaggedValue::Null; 
		}
	};

	template<>
	struct value_action< json::true_ >
	{
		template<typename Input>
		static void apply(Input const&, result_state& result)
		{
			result.result->type = Node::JsonType::Value;
			result.result->value = TaggedValue(true);
		}
	};

	template<>
	struct value_action< json::false_ >
	{
		template<typename Input>
		static void apply(Input const&, result_state& result)
		{
			result.result->type = Node::JsonType::Value;
			result.result->value = TaggedValue(false);
		}
	};

	template<>
	struct value_action< json::frac >
	{
		template<typename Input>
		static void apply(Input const& in, result_state& result)
		{
			result.result->value.tag = TaggedValue::Type::Float;
		}
	};

	template<>
	struct value_action< json::number >
	{
		template<typename Input>
		static void apply(Input const& in, result_state& result)
		{
			result.result->type = Node::JsonType::Value;
			if (result.result->value.tag == TaggedValue::Type::Float)
				result.result->value = TaggedValue(std::stod(in.string()));
			else
				result.result->value = TaggedValue(std::stoll(in.string()));
		}
	};

	// State and action classes to accumulate the data for a JSON array.

	struct array_state
		: public result_state
	{
		Node* array_result;

		array_state()
		{
			array_result = nullptr;
		}

		void push_back()
		{
			if (array_result == nullptr)
				array_result = new Node(Node::JsonType::Array);

			array_result->collect.push_back(result);

			result = new Node(Node::JsonType::None);
		}

		void success(result_state& in_result)
		{
			if (array_result == nullptr)
				array_result = new Node(Node::JsonType::Array);

			if (this->result->type != Node::JsonType::None)
			{
				push_back();
			}
			in_result.result = array_result;
		}
	};

	template< typename Rule >
	struct array_action
		: nothing< Rule >
	{
	};

	template<>
	struct array_action< json::value_separator >
	{
		template<typename Input>
		static void apply(Input const&, array_state& result)
		{
			result.push_back();
		}
	};

	// State and action classes to accumulate the data for a JSON object.

	struct object_state
		: public result_state
	{
		std::string unescaped;
		Node* object_result;

		object_state()
		{
			object_result = nullptr;
		}

		void insert()
		{
			if (object_result == nullptr)
				object_result = new Node(Node::JsonType::Object);

			auto* slotNode = new Node(Node::JsonType::Slot);
			slotNode->value = TaggedValue(unescaped);
			slotNode->collect.push_back(result);
			object_result->collect.push_back(slotNode);

			unescaped.clear();
			result = new Node(Node::JsonType::None);
		}

		void success(result_state& in_result)
		{
			if (this->result->type != Node::JsonType::None)
			{
				insert();
			}
			in_result.result = object_result;
		}
	};

	template< typename Rule >
	struct object_action
		: unescape_action< Rule >
	{
	};

	template<>
	struct object_action< json::value_separator >
	{
		template< typename Input >
		static void apply(Input const&, object_state& result)
		{
			result.insert();
		}
	};

	//
	// Final Grammar
	//

	template< typename Rule > struct control : errors< Rule > {};

	template<> struct control< json::value > : change_action< value, value_action, errors > {};
	template<> struct control< json::string::content > : change_state< json::string::content, string_state, errors > {};
	template<> struct control< json::array::content > : change_state_and_action< json::array::content, array_state, array_action, errors > {};
	template<> struct control< json::object::content > : change_state_and_action< json::object::content, object_state, object_action, errors > {};

	struct grammar : must< json::text, tao::pegtl::eof > {};

	//
	// To String
	//

	void writeToString(Node* root, std::string& str)
	{
		if (root == nullptr)
		{
			str.append("null");
			return;
		}

		switch (root->type)
		{
		default:
		case Node::JsonType::None:
		{
			str.append("null");
			return;
		}
		case Node::JsonType::Array:
		{
			str.append("[");
			bool firstWrite = true;
			for (auto n : root->collect)
			{
				if (!firstWrite)
					str.append(", ");
				else
					firstWrite = false;
				writeToString(n, str);
			}
			str.append("]");
			return;
		}
		case Node::JsonType::Object:
		{
			str.append("{");
			bool firstWrite = true;
			for (auto n : root->collect)
			{
				if (!firstWrite)
					str.append(", ");
				else
					firstWrite = false;
				writeToString(n, str);
			}
			str.append("}");
			return;
		}
		case Node::JsonType::Slot:
		{
			str.append(root->slotKey() + ": ");
			writeToString(root->slotValue(), str);
			return;
		}
		case Node::JsonType::Value:
		{
			switch (root->value.tag)
			{
			default:
			case TaggedValue::Type::Null:
			case TaggedValue::Type::Unknown:
			{
				str.append("null");
				return;
			}
			case TaggedValue::Type::Bool:
			{
				str.append(root->value.value._bool ? "true" : "false");
				return;
			}
			case TaggedValue::Type::Integer:
			{
				str.append(std::to_string(root->value.value._int));
				return;
			}
			case TaggedValue::Type::Float:
			{
				str.append(std::to_string(root->value.value._float));
				return;
			}
			case TaggedValue::Type::String:
			{
				str.append(root->value.value._string);
				return;
			}
			}
		}
		}
	}

	instance<> makeValue(Node* n)
	{
		switch (n->value.tag)
		{
		case TaggedValue::Type::Bool: return (instance<>)instance<bool>::make(n->value.value._bool);
		case TaggedValue::Type::Integer: return (instance<>)instance<int64_t>::make(n->value.value._int);
		case TaggedValue::Type::Float: return (instance<>)instance<double>::make(n->value.value._float);
		case TaggedValue::Type::String: return (instance<>)instance<std::string>::make(n->value.value._string);
		default: return instance<>();
		}
	}

	void resurseMap(instance<Map> l, Node* n);
	void resurseList(instance<List> l, Node* n);

	void resurseMap(instance<Map> l, Node* n) 
	{
		for (auto kv : n->collect)
		{
			auto k = instance<std::string>::make(kv->slotKey());
			auto val = kv->slotValue();
			switch (val->type)
			{
			case Node::JsonType::None:
			{
				l->insert(k, instance<>());
				break;
			}
			case Node::JsonType::Value:
			{
				l->insert(k, makeValue(val));
				break;
			}
			case Node::JsonType::Object:
			{
				auto child = instance<Map>::make();
				resurseMap(child, val);
				l->insert(k, child);
				break;
			}
			case Node::JsonType::Array:
			{
				auto child = instance<List>::make();
				resurseList(child, val);
				l->insert(k, child);
				break;
			}
			default:
				break;
			}
		}
	}

	void resurseList(instance<List> l, Node* n)
	{
		for (auto kv : n->collect)
		{
			auto val = kv->slotValue();
			switch (val->type)
			{
			case Node::JsonType::None:
			{
				l->push(instance<>());
				break;
			}
			case Node::JsonType::Value:
			{
				l->push(makeValue(val));
				break;
			}
			case Node::JsonType::Object:
			{
				auto child = instance<Map>::make();
				resurseMap(child, val);
				l->push(child);
				break;
			}
			case Node::JsonType::Array:
			{
				auto child = instance<List>::make();
				resurseList(child, val);
				l->push(child);
				break;
			}
			default:
				break;
			}
		}
	}

}

void system::make_json_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	
	auto env = ns->environment();

	auto _json = instance<MultiMethod>::make();
	
	
	_json->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		json::result_state result;
		json::string_input<> s(*a, "");

		json::parse< json::grammar, json::nothing, json::control >(s, result);

		if (result.result->type == json::Node::JsonType::Object)
		{
			auto res = instance<Map>::make();
			json::resurseMap(res, result.result);
			return (instance<>)res;
		}

		else if (result.result->type == json::Node::JsonType::Array)
		{
			auto res = instance<List>::make();
			json::resurseList(res, result.result);
			return (instance<>)res;
		}
		else if (result.result->type == json::Node::JsonType::Value)
		{
			return json::makeValue(result.result);
		}
		else
		{
			return instance<>();
		}
	}));

	ret->define_eval("json", _json);
}
