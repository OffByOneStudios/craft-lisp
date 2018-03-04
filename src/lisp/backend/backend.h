#pragma once

#include "PBackend.hpp"

#include "BootstrapInterpreter.h"

namespace craft {
namespace lisp
{
	instance<PBackend> llvm_backend_provider();
	instance<PBackend> bootstrap_backend_provider();
}}
