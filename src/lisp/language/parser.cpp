#include "lisp/common.h"

#include "tao/pegtl.hh"

#include "parser.h"

using namespace craft;
using namespace craft::features;
using namespace craft::features::languages;
using namespace craft::features::languages::lisp;

CRAFT_OBJECT_DEFINE(SexpSymbol)
{
	_.defaults();
}

/******************************************************************************
** PEGTL Implementation
******************************************************************************/
namespace craft {
namespace features {
namespace languages {
namespace lisp {
namespace _details
{
  struct line_comment
     : pegtl::until< pegtl::eolf > {};

  struct list;

  struct list_comment
     : pegtl::if_must< pegtl::at< pegtl::one< '(' >, pegtl::disable< list > > > {};

  struct comment
     : pegtl::if_must< pegtl::one< '#' >, pegtl::sor< list_comment, line_comment > > {};

  struct nothing
     : pegtl::sor< pegtl::space, comment > {};

  struct number
     : pegtl::plus< pegtl::digit > {};

  struct symbol
     : pegtl::identifier {};

  struct quote
     : pegtl::seq<pegtl::one<'\''>, pegtl::plus<pegtl::alnum>> {};

  struct atom
     : pegtl::sor< number, quote, symbol > {};

  struct anything;

  struct list
     : pegtl::if_must< pegtl::one< '(' >, pegtl::until< pegtl::one< ')' >, anything > > {};

  struct something
     : pegtl::sor< atom, list > {};

  struct anything
     : pegtl::sor< nothing, something > {};

  struct file
     : pegtl::until< pegtl::eof, anything > {};

    template< typename Rule >
  	struct action
  		: pegtl::nothing< Rule >
  	{
  	};

    template<> struct action< _details::line_comment >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("line_comment: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::list_comment >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("list_comment: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::comment >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("comment: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::nothing >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("nothing: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::quote >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        std::string s(in.string(), 1);
        craft::instance<std::string> res = craft::instance<std::string>::make(s);
        context._stack.push(res);
  	 	}
  	};

    template<> struct action< _details::number >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        auto i = std::atoi(in.string().c_str());
        craft::instance<int32_t> res = craft::instance<int32_t>::make(i);
        context._stack.push(res);
  	 	}
  	};

    template<> struct action< _details::symbol >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        lisp::SexpSymbol l;
        l.s = std::string(in.string());
				craft::instance<lisp::SexpSymbol> res = craft::instance<lisp::SexpSymbol>::make(l);
        context._stack.push(res);
  	 	}
  	};

    template<> struct action< _details::atom >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("atom: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::list >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("list: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::something >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("something: {0}", in.string());
  	 	}
  	};

    template<> struct action< _details::anything >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("anything: {0}", in.string());
  	 	}
  	};
  	template<> struct action< _details::file >
   	{
   		template< typename input >
   		static void apply(const input in, ParseContext& context)
   		{
        //context._logger->info("file: {0}", in.string());
  	 	}
  	};
}}}}}

void lisp::Evaluate(std::string input, instance<> root,  std::shared_ptr<spdlog::logger> logger)
{
	if (input == "") return;

	ParseContext p;
  p._logger = logger;
	try
	{
		bool res = pegtl::parse< _details::file, _details::action >(input, "<console>", p);
		while(p._stack.size())
    {
      instance<> i = p._stack.top();
      p._stack.pop();
      TypeId t = i.typeId();
      if(type<std::string>::typeId() == t)
      {
        std::string s = *i.asType<std::string>().get();
        logger->info("String:{0}", s);
      }
      else if(type<std::int32_t>::typeId() == t)
      {
        int32_t s = *i.asType<int32_t>().get();
        logger->info("Int:{0}", s);
      }
      else if(type<SexpSymbol>::typeId() == t)
      {
        SexpSymbol* s = i.asType<SexpSymbol>().get();
        logger->info("Symbol:{0}", s->s);
      }
    }

	}
	catch (std::exception e)
	{
		throw stdext::exception(e, "Parse Error");
	}


	return;
}
