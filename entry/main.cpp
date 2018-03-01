#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include <spdlog/spdlog.h>
#include <bandit/bandit.h>

using namespace bandit;
using namespace craft;
using namespace craft::lisp;

// the default read-eval-print-loop
void repl(const std::string & prompt, instance<Environment> env)
{
	std::string long_line = "";

	for (;;)
	{
		if (long_line.empty())
			std::cout << prompt;
		else
			std::cout << std::string(prompt.size(), '.');

		std::string line;
		std::getline(std::cin, line);
		long_line += line;

		instance<Sexpr> top_level;
		try
		{
			top_level = env->read(env->ns_user, long_line);
		}
		catch (std::exception const& e)
		{
			std::cout << e.what() << '\n';

			if (line.empty())
				long_line = "";

			continue;
		}

		long_line = "";

		try
		{
			if (!top_level->cells.empty())
			{
				auto exec = instance<Execution>::make(env, env->ns_user);
				std::cout << env->eval(instance<Frame>::make(exec), top_level->cells[0]).toString() << '\n';
			}
		}
		catch (std::exception const& e)
		{
			std::cout << e.what() << '\n';
		}
	}
}

int main()
{
	::craft::types::system().init();
	instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"));
	//add_globals(global_env);
	repl("CULT> ", global_env);
}