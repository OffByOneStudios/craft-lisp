
#include "lisp/common.h"
#include "lisp/library/libraries.h"


using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_regex_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto _rmatch = instance<MultiMethod>::make();
	_rmatch->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string, bool>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));

		std::regex r(*a);

		std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
		std::regex_match(*b, sm, r);

		return instance<bool>::make(sm.size() != 0);

	}));
	ret->define_eval("rmatch", _rmatch);
}
