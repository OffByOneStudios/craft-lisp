#include "tao/pegtl.hpp"

#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/parser.h"

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

	// Comments are introduced by a ';' and proceed to the end-of-line/file,
	// or until the matching paren, e.g. ;(comment)
	struct line_comment : until< eolf > {};
	struct sexpr_comment : if_must< at< sexpr_open >, disable< sexpr > > {};

	struct comment : if_must< one< ';' >, sor< sexpr_comment, line_comment > > {};

	// Nothing combination
	struct nothing : sor< space, comment > {};

	// Atom types
	struct lisp_ident_char : utf8::not_one < ')', '"', ';', ' ', '\n', '\r', '\t', '\v', '\f' > {};
	struct lisp_ident : plus < lisp_ident_char > {};

	struct symbol : lisp_ident {};
	struct keyword : seq< one< ':' >, lisp_ident > {};

	struct number : seq< opt< one < '-' > >, digit, star < lisp_ident_char > > {};
	struct string : if_must< one< '"' >, until< one< '"' >, star < utf8::not_one < '"' > > > > {};

	struct atom : sor< string, number, keyword, symbol > {};

	// List type
	struct sexpr : if_must< sexpr_open, until< sexpr_close, anything > >{};
	
	// Meta
	struct something : sor< sexpr, atom > {};
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
		static void apply0(ParseStack& ps)
		{
			ps.push(instance<Sexpr>::make());
		}
	};

	template<>
	struct lisp_action< sexpr_close >
	{
		static void apply0(ParseStack& ps)
		{
			auto finished = ps.top();
			ps.pop();

			ps.top()->cells.push_back(finished);
		}
	};

	template<>
	struct lisp_action< keyword >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cells.push_back(instance<Keyword>::make(in.string()));
		}
	};

	template<>
	struct lisp_action< symbol >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			ps.top()->cells.push_back(instance<Symbol>::make(in.string()));
		}
	};

	template<>
	struct lisp_action< number >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			std::string s = in.string();
			// TODO specialized parse function returning arbitrary percision number
			// and/or deciding on the prefered type/size of the number
			if (s.find('.', 0) != std::string::npos)
				ps.top()->cells.push_back(craft::types::type<double>::typeId().getFeature<PParse>()->parse(in.string()));
			else
				ps.top()->cells.push_back(craft::types::type<int64_t>::typeId().getFeature<PParse>()->parse(in.string()));
		}
	};

	template<>
	struct lisp_action< string >
	{
		template<typename Input>
		static void apply(Input const& in, ParseStack& ps)
		{
			std::string pstr = in.string();
			ps.top()->cells.push_back(instance<std::string>::make(pstr.substr(1, pstr.size() - 2)));
		}
	};
}

instance<Sexpr> craft::lisp::parse_lisp(std::string const& text)
{
	using namespace lisp_grammar;

	auto root = instance<Sexpr>::make();
	ParseStack parse_stack;
	parse_stack.push(root);

	string_input<> in(text, std::string(""));
	parse< grammar, lisp_action >(in, parse_stack);

	if (parse_stack.size() != 1)
		throw stdext::exception("Parse went horribly wrong.");

	return root;
}
