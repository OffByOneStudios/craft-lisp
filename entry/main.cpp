#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include "replxx/replxx.hxx"
#include "lisp/semantics/cult/cult.h"
#include "lisp/library/system/prelude.h"
#include "lisp/features/repl.h"

using namespace craft;
using namespace craft::lisp;





int main(int argc, char** argv)
{
	craft::types::boot();

	if (argc != 1)
	{
		instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"));
		instance<Namespace> ns = global_env->ns_user;
		std::string f;
		try
		{
			f = fs::read<std::string>(path::normalize(argv[1]), fs::string_read).get();
		}
		catch (...)
		{
			global_env->log()->info("No Such File: {0}", argv[1]);
		}
		try
		{
			auto live_module = ns->requireModule(fmt::format("file:{0}", f));
			live_module->initialize();
		}
		catch (stdext::exception e)
		{
			global_env->log()->error(e.what());
			return -1;
		}
		catch (std::exception e)
		{
			global_env->log()->error(e.what());
			return -1;
		}
	}
	else
	{
		auto repl = instance<features::LispRepl>::make();
		while (repl->step()) {}
	}

	return 0;
	
	
	//add_globals(global_env);
	
}
