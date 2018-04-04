#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp {
namespace library
{

	class Buffer
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Buffer);
	protected:
		std::vector<uint8_t> _data;

	public:
		CRAFT_LISP_EXPORTED Buffer(instance<int64_t> size);
		CRAFT_LISP_EXPORTED instance<int64_t> size();
		CRAFT_LISP_EXPORTED instance<int64_t> at(instance<int64_t> i);
		CRAFT_LISP_EXPORTED void set(instance<int64_t> i, instance<int64_t> d);
		CRAFT_LISP_EXPORTED std::vector<uint8_t>& data();
		CRAFT_LISP_EXPORTED instance<Buffer> join(instance<Buffer> other);
	};

	class List
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::List);
	protected:
		std::vector<instance<>> _data;

	public:

		CRAFT_LISP_EXPORTED instance<int64_t> size();
		CRAFT_LISP_EXPORTED instance<> at(instance<int64_t> i);
		CRAFT_LISP_EXPORTED void insert(instance<int64_t> i, instance<> v);
		CRAFT_LISP_EXPORTED void erase(instance<int64_t> i);
		CRAFT_LISP_EXPORTED void reverse();
		CRAFT_LISP_EXPORTED void push(instance<> i);
		CRAFT_LISP_EXPORTED instance<> pop();
		CRAFT_LISP_EXPORTED instance<List> slice(instance<int64_t> i, instance<int64_t> j);
		CRAFT_LISP_EXPORTED std::vector<instance<>>& data();
	};

	class Map
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Map);
	private:
		struct cmpByInstanceSemantics {
			inline bool operator()(const instance<>& a, const instance<>& b) const {
				if (a.typeId() != b.typeId()) return a.typeId() < b.typeId();

				if (a.typeId().isType<std::string>())
					return *a.asType<std::string>() < *b.asType<std::string>();
				else if (a.typeId().isType<int64_t>())
					return *a.asType<int64_t>() < *b.asType<int64_t>();
				else return a.get() < b.get();
			}
		};
	protected:
		std::map<instance<>, instance<>, cmpByInstanceSemantics> _data;

	public:

		CRAFT_LISP_EXPORTED instance<int64_t> size();

		CRAFT_LISP_EXPORTED instance<> at(instance<> i);

		CRAFT_LISP_EXPORTED void insert(instance<> i, instance<> v);
		CRAFT_LISP_EXPORTED void erase(instance<> i);
		CRAFT_LISP_EXPORTED std::map<instance<>, instance<>, cmpByInstanceSemantics>& data();
		CRAFT_LISP_EXPORTED std::vector<instance<>> keys();
		CRAFT_LISP_EXPORTED std::vector<instance<>> values();
	};

	class HttpServer
		: public craft::net::HttpServer
		, public virtual types::Object

	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::HttpServer);

	public:
		CRAFT_LISP_EXPORTED HttpServer(std::shared_ptr<spdlog::logger> logger, int port);

		inline virtual ~HttpServer() {};
	};

	class HttpEchoServer
		: public craft::net::HTTPRequestHandler
	{
		
	private:
	public:
		CRAFT_LISP_EXPORTED virtual bool handle(net::HTTPRequest& req, net::HttpResponse& rep) override;
	};
}}}

namespace json {
	typedef uint64_t Identifier;

	struct TaggedValue
	{
		enum class Type
			: int64_t
		{
			Unknown = -1,

			Null = 0,
			String = 1,
			Bool = 2,
			Integer = 3,
			Float = 4,
		};

		union Value
		{
			void* _null;
			std::string _string;
			bool _bool;
			int64_t _int;
			double _float;

			Value(void* v) : _null(v) { }
			Value(std::string v) : _string(v) { }
			Value(bool v) : _bool(v) { }
			Value(int64_t v) : _int(v) { }
			Value(double v) : _float(v) { }

			~Value() { }

			inline void construct(Type tag)
			{
				switch (tag)
				{
				case Type::String: new(&_string) std::string(); break;
				case Type::Unknown:
				case Type::Null:
				case Type::Bool:
				case Type::Integer:
				case Type::Float:
					break;
				}
			}

			inline void copy(Type tag, Value const& v)
			{
				switch (tag)
				{
				case Type::Null: _null = v._null; break;
				case Type::Bool: _bool = v._bool; break;
				case Type::Integer: _int = v._int; break;
				case Type::Float: _float = v._float; break;
				case Type::String: _string = v._string; break;
				case Type::Unknown:
					break;
				}
			}
			inline void destroy(Type tag)
			{
				switch (tag)
				{
				case Type::String: _string.~basic_string(); break;
				case Type::Unknown:
				case Type::Null:
				case Type::Bool:
				case Type::Integer:
				case Type::Float:
					break;
				}
			}
		};

		Value value;
		Type tag;

		TaggedValue() : value(nullptr), tag(Type::Unknown) { }
		TaggedValue(void* ptr) : value(ptr), tag(Type::Null) { }
		explicit TaggedValue(bool const& v) : value(v), tag(Type::Bool) { }
		TaggedValue(int64_t const& v) : value(v), tag(Type::Integer) { }
		TaggedValue(double const& v) : value(v), tag(Type::Float) { }
		TaggedValue(char const* v) : value(std::string(v)), tag(Type::String) { }
		TaggedValue(std::string const& v) : value(v), tag(Type::String) { }

		TaggedValue(TaggedValue const& that)
			: value(nullptr), tag(that.tag)
		{
			value.construct(tag);
			value.copy(tag, that.value);
		}

		TaggedValue const& operator=(TaggedValue const& that)
		{
			value.destroy(tag);
			tag = that.tag;
			value.construct(tag);
			value.copy(tag, that.value);

			return *this;
		}

		~TaggedValue() { value.destroy(tag); }

		CRAFT_LISP_EXPORTED static const TaggedValue Null;
	};

	enum class TypeDescriptionEnum
		: int64_t
	{
		None = 0,
		BuiltIn = 1,
		Dynamic = 2,

		Ns0 = 1 << 16,

		Unknown = -1,
	};

	enum class ValueDescriptionEnum
		: int64_t
	{
		Null = (int64_t)TaggedValue::Type::Null,
		String = (int64_t)TaggedValue::Type::String,
		Bool = (int64_t)TaggedValue::Type::Bool,
		Integer = (int64_t)TaggedValue::Type::Integer,
		Float = (int64_t)TaggedValue::Type::Float,

		Container = 256,

		Unknown = -1,
		SpecialReadable = -2,
		SpecialUnknown = -3,
	};

	enum class ContainerDescriptionFlags
		: uint64_t
	{
		Scalar = 0,

		Flag_DataStructure = 1 << 0,

		Flag_Serializeable = 1 << 8,
		Flag_RandomAccess = 1 << 9,

		Flag_HashUnique = 1 << 16,
		Flag_Mapping = 1 << 17,

		// Helper enums
		List = Flag_DataStructure | Flag_Serializeable,
		Array = Flag_DataStructure | Flag_Serializeable | Flag_RandomAccess,
		Set = Flag_DataStructure | Flag_HashUnique,
		Map = Flag_DataStructure | Flag_HashUnique | Flag_Mapping,
		OrderedMap = Flag_DataStructure | Flag_HashUnique | Flag_Mapping | Flag_Serializeable,
	};

	enum class InsertBehaviorEnum
		: uint64_t
	{
		Default = 0,
		Overwrite = 1,
		InsertOnly = 8,
		AppendBefore = 10,
		AppendAfter = 11
	};
	
}