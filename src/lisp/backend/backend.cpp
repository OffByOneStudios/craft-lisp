#include "lisp/common.h"
#include "lisp/backend/backend.h"
#include "lisp/backend/llvm/llvm_internal.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(PBackend) { _.defaults(); }


instance<PBackend> craft::lisp::llvm_backend_provider()
{
	return instance<PBackend>::forType<LlvmBackend>();
}

instance<PBackend> craft::lisp::bootstrap_backend_provider()
{
	return instance<PBackend>::forType<BootstrapInterpreter>();
}
