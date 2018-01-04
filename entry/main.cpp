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
	for (;;)
	{
		std::cout << prompt;
		std::string line; std::getline(std::cin, line);
		try
		{
			instance<Sexpr> top_level = env->read(env->ns_user, line);
			std::cout << env->eval(env->ns_user, top_level->cells[0]).toString() << '\n';
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