
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "lisp/backend/llvm/llvm_internal.h"
#include "prelude.h"
using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void core::make_llvm_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("llvm/ir",
		[](instance<Function> a) -> instance<std::string>
	{
		auto ret = instance<std::string>::make("");
		/*llvm::raw_string_ostream l(*ret);

	
		a->backend.asType<LlvmSubroutine>()->func->print(l);

		if (a->backend)
		{
			a->backend.asType<LlvmSubroutine>()->func->print(l);
		}
		else
		{
			ret->append("No IR.");
		}
*/
		return ret;
	});

	semantics->builtin_implementMultiMethod("llvm/ir",
		[](instance<Module> a) -> instance<std::string>
	{
		auto ret = instance<std::string>::make("");
		/*llvm::raw_string_ostream l(*ret);
		a->backend.asType<LlvmModule>()->ir->print(l, nullptr);

		if (a->backend)
		{
			a->backend.asType<LlvmModule>()->ir->print(l, nullptr);
		}
		else
		{
			ret->append("No IR.");
		}

		return ret;*/
		return ret;
	});
}
