#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/parser.h"

#include "tao/pegtl.hpp"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

namespace lisp_grammar
{
	// Actual grammar

	// Use grammar library
	using namespace tao::pegtl;

	// Forward declare
	struct sexpr;
	struct anything;

	// sexpr primitives
	struct sexpr_open : one< '(' > {};
	struct sexpr_close : one < ')' > {};

	// vector primitives
	struct vector_open : one< '[' > {};
	struct vector_close : one < ']' > {};

	// map primitives
	struct map_open : one< '{' > {};
	struct map_close : one < '}' > {};

	// Comments are introduced by a ';' and proceed to the end-of-line/file,
	// or until the matching paren, e.g. ;(comment)
	struct line_comment : until< eolf > {};
	struct sexpr_comment : if_must< at< sexpr_open >, disable< sexpr > > {};

	struct comment : if_must< one< ';' >, sor< sexpr_comment, line_comment > > {};

	// Nothing combination
	struct nothing : sor< space, comment > {};

	// Atom types
	struct lisp_ident_char : utf8::not_one < ')', '"', ';', ' ', '{', '}', '\n', '\r', '\t', '\v', '\f' > {};
	struct lisp_ident : plus < lisp_ident_char > {};

	struct symbol : lisp_ident {};

	struct floatsize : seq <one<'f'>, sor<
		string<'3', '2'>,
		string<'6', '4'>,
		string<'i'>
		>> {};

	struct uintsize : seq <one<'u'>, opt<
		sor<string<'8'>,
		string<'1', '6'>,
		string<'3', '2'>,
		string<'6', '4'>
		>>> {};

	struct intsize : seq <one<'i'>, opt<sor<
		string<'8'>,
		string<'1', '6'>,
		string<'3', '2'>,
		string<'6', '4'>,
		string<'i'>
		>>> {};

	struct floatliteral : seq< opt< one < '-' > >, plus<digit>, one<'.'>, plus<digit>, opt<floatsize>> {};
	struct intliteral : seq< opt< one < '-' > >, plus<digit>, opt<intsize>> {};
	struct uintliteral : seq< plus<digit>, uintsize> {};
	struct number : sor<floatliteral, uintliteral, intliteral> {};

	struct null : tao::pegtl::string< 'n', 'u', 'l', 'l' > {};
	struct true_ : tao::pegtl::string< 't', 'r', 'u', 'e' > {};
	struct false_ : tao::pegtl::string < 'f', 'a', 'l', 's', 'e'> {};
	

	struct triplequote : tao::pegtl::string<'"', '"', '"'> {};

	struct triplestring : if_must< triplequote, until<triplequote> > {};
	struct singlestring : if_must< one< '"' >, until< one< '"' > > > {};
	struct strings : sor<triplestring, singlestring>{};

	struct atom : sor<null, true_, false_, strings, number, symbol > {};

	// List type
	struct sexpr : if_must< sexpr_open, until< sexpr_close, anything > >{};
	struct vector : if_must< vector_open, until< vector_close, anything > > {};
	struct map : if_must< map_open, until< map_close, anything > > {};
	// Meta
	struct something : sor< vector, map, sexpr, atom > {};
	struct anything : sor< nothing, something > {};

	struct grammar : until< tao::pegtl::eof, anything > {};

	// States
	typedef std::stack<instance<Sexpr>> ParseStack;

	// Actions

	template< typename Rule >
	struct lisp_action
		: tao::pegtl::nothing< Rule >
	{};
	
	template<>
	struct lisp_action< sexpr_open >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			auto new_expr = instance<Sexpr>::make();
			new_expr->cell_locs.push_back(in.position().byte);

			ps.push(new_expr);
		}
	};

	template<>
	struct lisp_action< sexpr_close >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			auto finished = ps.top();
			ps.pop();
			finished->cell_locs.push_back(in.position().byte);

			ps.top()->cells.push_back(finished);
		}
	};

	template<>
	struct lisp_action< vector_open >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			auto new_expr = instance<Sexpr>::make();
			new_expr->cells.push_back(Symbol::makeSymbol("list"));
			new_expr->cell_locs.push_back(in.position().byte);

			ps.push(new_expr);
		}
	};

	template<>
	struct lisp_action< vector_close >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			auto finished = ps.top();
			ps.pop();
			finished->cell_locs.push_back(in.position().byte);

			ps.top()->cells.push_back(finished);
		}
	};

	template<>
	struct lisp_action< map_open >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			auto new_expr = instance<Sexpr>::make();
			new_expr->cells.push_back(Symbol::makeSymbol("map"));
			new_expr->cell_locs.push_back(in.position().byte);

			ps.push(new_expr);
		}
	};

	template<>
	struct lisp_action< map_close >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			auto finished = ps.top();
			ps.pop();
			finished->cell_locs.push_back(in.position().byte);

			ps.top()->cells.push_back(finished);
		}
	};

	template<>
	struct lisp_action< symbol >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cell_locs.push_back(in.position().byte);
			ps.top()->cells.push_back(Symbol::makeSymbol(in.string()));
		}
	};

	template<>
	struct lisp_action< uintliteral >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cell_locs.push_back(in.position().byte);
			std::string s = in.string();
			auto ind = s.find('u');
			if (ind == s.npos)
			{
				ps.top()->cells.push_back(craft::types::cpptype<uint64_t>::typeDesc().getFeature<PParse>()->parse(s));
			}
			else
			{
				auto suf = s.substr(ind);
				if (suf == "") ps.top()->cells.push_back(craft::types::cpptype<uint64_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "u8") ps.top()->cells.push_back(craft::types::cpptype<uint8_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "u16") ps.top()->cells.push_back(craft::types::cpptype<uint16_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "u32") ps.top()->cells.push_back(craft::types::cpptype<uint32_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "u64") ps.top()->cells.push_back(craft::types::cpptype<uint64_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else throw stdext::exception("No Such Integer Literal Suffix: {0}", suf);
			}

		}

	};
	template<>
	struct lisp_action< intliteral >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cell_locs.push_back(in.position().byte);
			std::string s = in.string();
			auto ind = s.find('i');
			if (ind == s.npos)
			{
				ps.top()->cells.push_back(craft::types::cpptype<int64_t>::typeDesc().getFeature<PParse>()->parse(s));
			}
			else
			{
				auto suf = s.substr(ind);
				if (suf == "") ps.top()->cells.push_back(craft::types::cpptype<int64_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "i8") ps.top()->cells.push_back(craft::types::cpptype<int8_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "i16") ps.top()->cells.push_back(craft::types::cpptype<int16_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "i32") ps.top()->cells.push_back(craft::types::cpptype<int32_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "i64") ps.top()->cells.push_back(craft::types::cpptype<int64_t>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "ii") ps.top()->cells.push_back(craft::types::cpptype<mpz_class>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else throw stdext::exception("No Such Integer Literal Suffix: {0}", suf);
			}
		}
	};

	template<>
	struct lisp_action< floatliteral >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cell_locs.push_back(in.position().byte);
			std::string s = in.string();
			auto ind = s.find('f');
			if (ind == s.npos)
			{
				ps.top()->cells.push_back(craft::types::cpptype<double>::typeDesc().getFeature<PParse>()->parse(s));
			}
			else
			{
				auto suf = s.substr(ind);
				if (suf == "") ps.top()->cells.push_back(craft::types::cpptype<double>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "f32") ps.top()->cells.push_back(craft::types::cpptype<float>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "f64") ps.top()->cells.push_back(craft::types::cpptype<double>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else if (suf == "fi") ps.top()->cells.push_back(craft::types::cpptype<mpf_class>::typeDesc().getFeature<PParse>()->parse(s.substr(0, ind)));
				else throw stdext::exception("No Such Float Literal Suffix: {0}", suf);
			}
		}
	};

	template<>
	struct lisp_action< false_ >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			std::string pstr = in.string();
			ps.top()->cells.push_back(instance<bool>::make(false));
		}
	};

	template<>
	struct lisp_action< true_ >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			std::string pstr = in.string();
			ps.top()->cells.push_back(instance<bool>::make(true));
		}
	};

	template<>
	struct lisp_action< null >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			std::string pstr = in.string();
			ps.top()->cells.push_back(instance<void>());
		}
	};

	template<>
	struct lisp_action< triplestring >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			std::string pstr = in.string();
			ps.top()->cells.push_back(instance<std::string>::make(pstr.substr(1 * 3, pstr.size() - 2 * 3)));
		}
	};

	template<>
	struct lisp_action< singlestring >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cell_locs.push_back(in.position().byte);
			std::string pstr = in.string();
			ps.top()->cells.push_back(instance<std::string>::make(pstr.substr(1, pstr.size() - 2)));
		}
	};
}

std::vector<instance<>> craft::lisp::parse_lisp(std::string const& text, bool add_source_locations)
{
	using namespace lisp_grammar;

	auto root = instance<Sexpr>::make();
	ParseStack parse_stack;
	parse_stack.push(root);

	string_input<> in(text, std::string(""));
	parse< grammar, lisp_action >(in, parse_stack);

	if (parse_stack.size() != 1)
		throw stdext::exception("Parse went horribly wrong.");

	return root->cells;
}



std::stack<instance<Sexpr>> craft::lisp::partial_parse(std::string const& text)
{
	using namespace lisp_grammar;

	auto root = instance<Sexpr>::make();
	ParseStack parse_stack;
	parse_stack.push(root);

	string_input<> in(text, std::string(""));

	try
	{
		parse< grammar, lisp_action >(in, parse_stack);
	}
	catch (...)
	{

	}
	
	return parse_stack;
}