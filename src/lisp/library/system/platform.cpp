#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


CRAFT_DEFINE(File)
{
	_.defaults();
}

CRAFT_DEFINE(File::Iterator)
{
	_.defaults();
}


// TODO a lot of this should move to our platform extension
void core::make_platform_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("platform/triplecross",
		[]() -> instance<std::string>
	{
		return instance<std::string>::make("Unknown");
	});

	semantics->builtin_implementMultiMethod("open",
		[](instance<std::string> filename, instance<Symbol> mode)
	{
		bool text;
		if (!mode->isKeyword() && mode->getDisplay() != ":text")
		{
			
		}

	});

	// Files support the iteration protocal
	semantics->builtin_implementMultiMethod("begin",
		[](instance<File> filename)
	{

	});

	// Files support the iteration protocal
	semantics->builtin_implementMultiMethod("increment",
		[](instance<File::Iterator> filename)
	{

	});

	// Files support the iteration protocal
	semantics->builtin_implementMultiMethod("current",
		[](instance<File::Iterator> filename)
	{

	});
}
