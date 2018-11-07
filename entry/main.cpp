#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include "replxx.hxx"
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
			_argv.push_back(instance<std::string>::make(argv[i]));
		}

		instance<Environment> env = instance<Environment>::make(spdlog::stdout_color_mt("environment"), _argv);

		try
		{
			auto live_module = env->importModule(instance<>(), fmt::format("file:{0}", path::normalize(argv[1])));
			live_module->initialize();
		}
		catch (std::exception const& e)
		{
			env->log()->error(e.what());
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
			catch (features::ReplParseException const& e)
			{
				std::cout << "Parse Error: " << e.what() << "\n";
			}
			catch (std::exception const& e)
			{
				std::cout << e.what() << "\n";
			}
			catch (...)
			{
				std::cout << "Unhandled Internal Exception... " << "\n";
			}
		}
	}

	return 0;
	
	
	//add_globals(global_env);
	
}
