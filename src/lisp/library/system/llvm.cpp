
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "lisp/backend/llvm/llvm_internal.h"
#include "prelude.h"
using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void system::make_llvm_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto llvm_ir = instance<MultiMethod>::make();
	llvm_ir->attach(ns->environment(), instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<Function, std::string>(),
		[](auto frame, auto args)
	{
		instance<Function> a(expect<Function>(args[0]));
		auto ret = instance<std::string>::make("");
		llvm::raw_string_ostream l(*ret);

		a->backend.asType<LlvmSubroutine>()->func->print(l);

		if (a->backend)
		{
			a->backend.asType<LlvmSubroutine>()->func->print(l);
		}
		else
		{
			ret->append("No IR.");
		}

		return ret;
	}));
	llvm_ir->attach(ns->environment(), instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<Module, std::string>(),
		[](auto frame, auto args)
	{
		instance<Module> a(expect<Module>(args[0]));
		auto ret = instance<std::string>::make("");
		llvm::raw_string_ostream l(*ret);
		a->backend.asType<LlvmModule>()->ir->print(l, nullptr);

		if (a->backend)
		{
			a->backend.asType<LlvmModule>()->ir->print(l, nullptr);
		}
			else
		{
			ret->append("No IR.");
		}

		return ret;
	}));
	ret->define_eval("llvm-ir", llvm_ir);
}
