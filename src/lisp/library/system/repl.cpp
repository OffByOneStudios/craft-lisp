
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_repl_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto _complete = instance<MultiMethod>::make();
	_complete->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, int64_t, List>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		instance<std::string> b(expect<int64_t>(args[1]));

		auto query = std::regex_replace(*a, std::regex("[()]"), "");
		auto choices = frame->getNamespace()->search(query);
		auto res = instance<List>::make();
		for (auto s : choices)
		{
			res->push(instance<std::string>::make(s->name()));
		}
		return res;

	}));
	ret->define_eval("replcomplete", _complete);

	
}
