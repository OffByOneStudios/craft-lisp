
#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include <spdlog/spdlog.h>
#include <bandit/bandit.h>



using namespace bandit;
using namespace craft;
using namespace craft::lisp;

// Tell bandit there are tests here.
go_bandit([](){
  describe("base:", [](){
    it("adds things", [&](){
        /*int four = craft::cmake::base::add(2, 2);
        AssertThat(four, Equals(4));*/
      });
  });
});

// the default read-eval-print-loop
void repl(const std::string & prompt, instance<Environment> env)
{
    for (;;) {
        std::cout << prompt;
        std::string line; std::getline(std::cin, line);
        try
        {
          std::cout << eval(read(line), env).to_string() << '\n';
        }
        catch(stdext::exception e)
        {
          std::cout << e.what() << '\n';
        }
    }
}

int main ()
{
    instance<Environment> global_env = instance<Environment>::make();
    add_globals(global_env);
    repl("ISTL> ", global_env);
}

//int main(int argc, char const *argv[]) {
//  auto logger = spdlog::stdout_color_mt("console");
//  craft::instance<std::string> s = craft::instance<std::string>::make(std::string("pLaceholder"));
//  std::string input("(concat 'answer (plus 2 (times 2 3)))");
//  craft::features::languages::lisp::Evaluate(input, s, logger);
//  //return bandit::run(argc, (char**)argv);
//}

