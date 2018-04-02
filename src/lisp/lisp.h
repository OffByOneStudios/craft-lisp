#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{
	class PBackend;
	class BootstrapInterpreter;

	class SScope;
	class SFrame;

	class Namespace;
	class Execution;
	class Environment;

	class Module;

	class SubroutineSignature;
	class Binding;
}}

namespace craft {
namespace types
{
  template <> struct type<craft::lisp::Environment>
    : public type<void>
  {
    static constexpr bool isObject = true;
    inline static TypeId typeId();// { return lisp::PBackend::craft_s_typeId(); }
  };
  
  template <> struct type<craft::lisp::Execution>
    : public type<void>
  {
    static constexpr bool isObject = true;
    inline static TypeId typeId();// { return lisp::PBackend::craft_s_typeId(); }
  };
  
  template <> struct type<craft::lisp::BootstrapInterpreter>
    : public type<void>
  {
    static constexpr bool isObject = true;
    inline static TypeId typeId();// { return lisp::PBackend::craft_s_typeId(); }
  };
  
  template <> struct type<craft::lisp::PBackend>
    : public type<void>
  {
    static constexpr bool isObject = true;
    inline static FeatureId featureId();// { return lisp::PBackend::craft_s_typeId(); }
  };
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

inline craft::types::TypeId craft::types::type<craft::lisp::Environment>::typeId()
{
  return craft::lisp::Environment::craft_s_typeId();
};

inline craft::types::TypeId craft::types::type<craft::lisp::Execution>::typeId()
{
  return craft::lisp::Execution::craft_s_typeId();
};

