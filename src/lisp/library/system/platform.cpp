#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

#include "util/platform_windows.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;
using namespace platform::windows;


void core::make_platform_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("platform/triplecross",
		[]() -> instance<std::string>
	{
		return instance<std::string>::make(CRAFT_TRIPLE_CROSS);
	});

	semantics->builtin_implementMultiMethod("platform/loaddll",
		[](instance<std::string> a)
	{

		
	});
}
