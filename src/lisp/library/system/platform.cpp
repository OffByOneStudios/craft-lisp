
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

namespace _impl {
#ifdef _WIN32
	extern std::string GetLastErrorAsString();
#endif
}

void core::make_platform_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("platform/triplecross",
		[]() -> instance<std::string>
	{
		return instance<std::string>::make(CRAFT_TRIPLE_CROSS);
	});

	semantics->builtin_implementMultiMethod("platform/loaddll",
		[](instance<std::string> a) -> instance<int64_t>
	{
		auto target = path::normalize(*a);
#ifdef _WIN32
		auto handle = LoadLibraryA(target.c_str());
		if (handle == nullptr) throw stdext::exception(_impl::GetLastErrorAsString());
		return instance<int64_t>::make(int64_t(handle));
#endif
	});
}
