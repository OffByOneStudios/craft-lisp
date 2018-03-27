#pragma once

#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	instance<PBackend> llvm_backend_provider();
	instance<PBackend> bootstrap_backend_provider();
}}
