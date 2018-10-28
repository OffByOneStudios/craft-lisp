#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class PSubroutine
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PSubroutine, "lisp.subroutine", types::SingletonProviderManager);

	public:
        
        CRAFT_LISP_EXPORTED virtual types::Function function(instance<>) const = 0;
		CRAFT_LISP_EXPORTED virtual types::ExpressionStore expression(instance<>) const = 0;

		//
		// Performs a full call on the subroutine, including pushing to the current execution
		//
		// TODO make this a multimethod: invoke(backend, object, call) -> result
		CRAFT_LISP_EXPORTED virtual instance<> execute(instance<>, types::GenericInvoke const& call) const = 0;
		
		// llvm-internal: ___cult__PSubroutine__runtime_execute
		static CRAFT_LISP_EXPORTED instance<> _cult_runtime_execute(instance<> subroutine, instance<>* args, size_t argc);
	};

	class SubroutineClosure
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::SubroutineClosure);
	public:
		// TODO make this an instance + offset when we clean up the shadow stack
		instance<> scope_frame;
		instance<PSubroutine> subroutine;
	
	public:
		CRAFT_LISP_EXPORTED SubroutineClosure(instance<> frame, instance<PSubroutine> subroutine);
	};
}}
