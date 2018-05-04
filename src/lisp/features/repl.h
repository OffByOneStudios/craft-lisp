#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"


#include "replxx/replxx.hxx"

namespace craft{
namespace lisp {
namespace features 
{
	class LispRepl
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::features::LispRepl);

		instance<Environment> _env;
		instance<Namespace> _ns;
		instance<Module> _live_module;
		std::string _history_file;

		replxx::Replxx _rx;
	public:

		CRAFT_LISP_EXPORTED LispRepl();


		CRAFT_LISP_EXPORTED bool step();
	};
}}}