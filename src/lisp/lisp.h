#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{

	class Scope;
	class Environment;

}}

#include "types.h"

#include "Keyword.h"
#include "Symbol.h"
#include "Sexpr.h"
#include "Binding.h"

#include "parser.h"

#include "SScope.hpp"
#include "Scope.h"
#include "Namespace.h"
#include "Module.h"
#include "Environment.h"

#include "PSubroutine.hpp"
#include "Function.h"
#include "BuiltinFunction.h"
#include "MultiMethod.h"
#include "Macro.h"
#include "SpecialForm.h"

#include "lisp/library/libraries.h"
