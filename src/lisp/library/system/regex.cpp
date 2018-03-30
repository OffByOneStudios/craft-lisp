
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

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

	auto _rfind = instance<MultiMethod>::make();
	_rfind->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));

		std::regex r(*a);

		std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
		std::regex_match(*b, sm, r);

		std::vector<instance<>> res;
		for (auto i : sm)
		{
			res.push_back(instance<std::string>::make(i.str()));
		}

		return frame->getNamespace()->lookup("list")->getValue(frame).asType<MultiMethod>()->call(frame, res);
	}));
	ret->define_eval("rfind", _rfind);
}
