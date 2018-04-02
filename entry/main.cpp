#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include <spdlog/spdlog.h>

using namespace craft;
using namespace craft::lisp;

// the default read-eval-print-loop
void repl(const std::string & prompt, instance<Environment> env)
{
	std::string long_line = "";

	auto live_module = env->ns_user->requireModule("repl:console");

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
			top_level = env->parse(env->ns_user, long_line);
		}
		catch (std::exception const& e)
		{
			if (line.empty())
			{
				long_line = "";
				std::cout << "parser: " << e.what() << '\n';
			}

			continue;
		}

		long_line = "";

		try
		{
			if (top_level)
			{
				std::cout << live_module->liveContinueWith(top_level).toString() << '\n';
			}
		}
		catch (std::exception const& e)
		{
			std::cout << e.what() << '\n';
		}
	}
}

int main(int argc, char** argv)
{
	::craft::types::system().init();
	instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"));
	if (argc == 1)
	{
		repl("CULT> ", global_env);
	}
	else
	{
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
			global_env->eval(f);
		}
		catch (std::exception e)
		{
			global_env->log()->error(e.what());
			return -1;
		}
	}

	return 0;
	
	
	//add_globals(global_env);
	
}
