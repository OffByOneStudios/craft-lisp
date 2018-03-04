#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/llvm/llvm_internal.h"
#include "lisp/backend/llvm/LlvmModule.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

using namespace llvm;
using namespace llvm::orc;

CRAFT_OBJECT_DEFINE(LlvmModule)
{
	_.defaults();
}

LlvmModule::LlvmModule(instance<LlvmBackend> backend, instance<lisp::Module> lisp)
{
	_parent = backend;
	_lisp = lisp;
}

void LlvmModule::generate()
{
	auto s = _lisp->uri();

	auto m = std::make_unique<llvm::Module>(StringRef(s), _parent->_context);

	ir = std::move(m);
}
