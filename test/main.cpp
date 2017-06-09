
#include "lisp/common.h"
#include "lisp/lisp.h"

#include <stack>
#include <queue>

#include <spdlog/spdlog.h>
#include <bandit/bandit.h>



using namespace bandit;


// Tell bandit there are tests here.
go_bandit([](){
  describe("base:", [](){
    it("adds things", [&](){
        /*int four = craft::cmake::base::add(2, 2);
        AssertThat(four, Equals(4));*/
      });
  });
});

int main(int argc, char const *argv[]) {
  auto logger = spdlog::stdout_color_mt("console");
  craft::instance<std::string> s = craft::instance<std::string>::make(std::string("pLaceholder"));
  std::string input("(concat 'answer (plus 2 (times 2 3)))");
  craft::features::languages::lisp::Evaluate(input, s, logger);
  //return bandit::run(argc, (char**)argv);
}
