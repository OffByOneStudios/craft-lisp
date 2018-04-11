#pragma once
#include "common.h"

namespace craft {
namespace lisp
{
	class BootstrapInterpreter;

	class SFrame;

	class Namespace;
	class Module;
	class Execution;
	class Environment;

	class Function;
	class MultiMethod;
}}

#include "exceptions.hpp"

#include "SymbolStore.hpp"
#include "GenericCall.hpp"

#include "PSyntax.hpp"
#include "PSemantics.hpp"
#include "PBackend.hpp"
#include "PExecutor.hpp"
#include "PCompiler.hpp"
#include "SFrame.hpp"

#include "Namespace.h"
#include "Module.h"
#include "Execution.h"
#include "Environment.h"

#include "lisp/syntax/syntax.h"

#include "backend/backend.h"

