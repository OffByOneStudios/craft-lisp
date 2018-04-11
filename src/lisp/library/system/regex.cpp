
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void core::make_regex_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("rmatch",
		[](instance<std::string> a, instance<std::string> b) -> instance<bool>
	{
		std::regex r(*a);

		std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
		std::regex_match(*b, sm, r);

		return instance<bool>::make(sm.size() != 0);

	});

	semantics->builtin_implementMultiMethod("rfind",
		[](instance<std::string> a, instance<std::string> b) -> instance<List>
	{
		std::regex r(*a);

		std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
		std::regex_match(*b, sm, r);

		std::vector<instance<>> res;
		for (auto i : sm)
		{
			res.push_back(instance<std::string>::make(i.str()));
		}

		return instance<List>::make(res);
	});
}
