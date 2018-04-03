
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


	auto _dllload = instance<MultiMethod>::make();
	_dllload->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, int64_t>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<std::string> a = expect<std::string>(args[0]);
		auto target = path::normalize(*a);
#ifdef _WIN32
		auto handle = LoadLibrary(target.c_str());
		if (handle == nullptr) throw stdext::exception(_impl::GetLastErrorAsString());
		return instance<int64_t>::make(int64_t(handle));
#endif
	}));

	ret->define_eval("dllload", _dllload);
}
