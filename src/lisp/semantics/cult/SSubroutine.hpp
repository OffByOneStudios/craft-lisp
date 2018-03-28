#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** SSubroutine
	******************************************************************************/

	/* Provides an interface for executable objects.

	Should cover:
	  * How the object should be executed.
	  * The signature and side effects of the execution.
	  * Access to the raw call.
	  * Helpers for building the stack frame information.
	*/

	class SSubroutine abstract
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(SSubroutine, "lisp.subroutine", craft::types::FactoryAspectManager);

	public:
		// Module this was originally defined in
		virtual instance<CultSemantics> getSemantics() const = 0;

		// Signature of the module
		virtual instance<SubroutineSignature> signature() const = 0;
	};
}}
