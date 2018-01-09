#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{
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

#include "SubroutineSignature.h"
#include "Function.h"
#include "Closure.h"
#include "BuiltinFunction.h"
#include "MultiMethod.h"
#include "Macro.h"
#include "SpecialForm.h"

#include "lisp/library/libraries.h"
