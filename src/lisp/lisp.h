#pragma once
#include "common.h"

namespace craft {
namespace lisp
{
	class BootstrapInterpreter;

	class SFrameSection;

	class Environment;
	class Module;
	class Execution;

	class Function;
	class MultiMethod;
}}

#include "exceptions.hpp"

#include "SymbolStore.hpp"
#include "syntax/Symbol.h"

#include "semantics/subroutine.h"
#include "semantics/slots.h"

#include "PSyntax.hpp"
#include "PSemantics.hpp"
#include "PBackend.hpp"
#include "PExecutor.hpp"
#include "PCompiler.hpp"
#include "SFrame.hpp"

#include "loaders.h"

#include "Environment.h"
#include "Module.h"
#include "Execution.h"

#include "lisp/syntax/syntax.h"
#include "lisp/library/system/prelude.h"

#include "lisp/semantics/cult/calling.h"

#include "backend/backend.h"

