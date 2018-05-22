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
		std::vector<instance<std::string>> _argv;
		for (auto i = 0; i < argc; i++)
		{
			_argv.push_back < instance<std::string>::make(argv[i]);
		}
		instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"), _argv);
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
			auto live_module = ns->requireModule(instance<>(), fmt::format("file:{0}", f));
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
		features::LispRepl r;

		while (true)
		{
			try
			{
				auto eval = r.step();
				std::cout << eval.toString() << "\n";
			}
			catch (features::ReplExitException e)
			{
				break;
			}
			catch (features::ReplParseException e)
			{
				std::cout << "Parse Error: " << e.what() << "\n";
			}
			catch (stdext::exception e)
			{
				std::cout << e.what() << "\n";
			}
			catch (std::exception e)
			{
				std::cout << "Unhandled Internal Exception: " << e.what() << "\n";
			}
		}
	}

	return 0;
	
	
	//add_globals(global_env);
	
}
