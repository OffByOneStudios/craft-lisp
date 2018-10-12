#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void core::make_platform_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("platform/triplecross",
		[]() -> instance<std::string>
	{
		return instance<std::string>::make("Unknown");
	});

}
