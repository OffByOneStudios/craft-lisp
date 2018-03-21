#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

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
		virtual instance<Module> getModule(instance<>) const = 0;

		virtual instance<SubroutineSignature> signature(instance<> subroutine) const = 0;

		virtual instance<SFrame> call_frame() const = 0;
		virtual instance<> call(instance<SFrame> const& call_frame, std::vector<instance<>> const&) const = 0;
	};
}}
