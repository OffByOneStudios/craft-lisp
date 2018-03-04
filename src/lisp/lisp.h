#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{
	class PBackend;

	class SScope;
	class SFrame;

	class Namespace;
	class Execution;
	class Environment;

	class SubroutineSignature;
	class Binding;
}}

#include "SBinding.hpp"
#include "SScope.hpp"
#include "SFrame.hpp"
#include "PSubroutine.hpp"

#include "types.h"

#include "Keyword.h"
#include "Symbol.h"
#include "Sexpr.h"
#include "Binding.h"

#include "parser.h"

#include "Scope.h"
#include "Frame.h"
#include "Namespace.h"
#include "Module.h"
#include "Execution.h"
#include "Environment.h"

#include "BlockBinding.h"
#include "ClosureBinding.h"
#include "Block.h"

#include "Variable.h"

#include "subroutine/SubroutineSignature.h"
#include "subroutine/Function.h"
#include "subroutine/Closure.h"
#include "subroutine/BuiltinFunction.h"
#include "subroutine/MultiMethod.h"
#include "subroutine/Macro.h"
#include "subroutine/SpecialForm.h"

#include "backend/backend.h"

#include "lisp/library/libraries.h"
