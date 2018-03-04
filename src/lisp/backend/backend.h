#pragma once

#include "PBackend.hpp"

namespace craft {
namespace lisp
{
	instance<PBackend> llvm_backend_provider();
}}