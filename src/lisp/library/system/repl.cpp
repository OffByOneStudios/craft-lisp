
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
		instance<int64_t> b(expect<int64_t>(args[1]));


		auto res = instance<List>::make();

		if (size_t(*b) < a->length())
		{
			auto query = a->substr(size_t(*b));
			auto choices = frame->getNamespace()->search(query);
			for (auto s : choices)
			{
				res->push(instance<std::string>::make(s->name()));
			}
		}
		else
		{
			auto lp = a->rfind('(');
			if (lp != a->npos)
			{
				auto n = a->find(' ', lp);
				auto symbol = a->substr(lp + 1, n);
			}
			

		}
		return res;
		

	}));
	ret->define_eval("replcomplete", _complete);

	
}
