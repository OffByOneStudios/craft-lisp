
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_platform_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto triplecross = instance<MultiMethod>::make();
	triplecross->attach(env, instance<BuiltinFunction>::make(
		[](instance<SFrame> frame, auto args)
	{	
		return instance<std::string>::make(CRAFT_TRIPLE_CROSS);
	}));

	ret->define_eval("triplecross", triplecross);
}
