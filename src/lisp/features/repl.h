#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"


#include "replxx.hxx"

namespace craft{
namespace lisp {
namespace features 
{
	class LispRepl
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::features::LispRepl);

		instance<Environment> _env;
		instance<Module> _live_module;
		std::string _history_file;

		replxx::Replxx _rx;
	public:

		CRAFT_LISP_EXPORTED LispRepl(std::function<void(instance<Module>)> init = nullptr);

		CRAFT_LISP_EXPORTED inline instance<Environment> getEnviroment() { return _env; }
		CRAFT_LISP_EXPORTED inline instance<Module> getModule() { return _live_module; }
		
		CRAFT_LISP_EXPORTED instance<> invoke(instance<std::string> s, bool record = false);
		CRAFT_LISP_EXPORTED instance<> step();
	};

	class ReplExitException : public stdext::exception {
	public:
		inline ReplExitException() : stdext::exception("Repl Exit") {};
	};

	class ReplParseException : public stdext::exception {
	public:
		inline ReplParseException(std::string what) : stdext::exception("Parse Error {0}", what)  {};
	};
}}}