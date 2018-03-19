#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** PSubroutine
	******************************************************************************/

	/* Provides an interface for executable objects.

	Should cover:
	  * How the object should be executed.
	  * The signature and side effects of the execution.
	  * Access to the raw call.
	  * Helpers for building the stack frame information.
	*/

	class PSubroutine abstract
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(PSubroutine, "lisp.subroutine", craft::types::SingletonProviderManager);

	public:
		virtual instance<SubroutineSignature> signature(instance<> subroutine) const = 0;

		virtual instance<SFrame> call_frame(instance<> subroutine) const = 0;
		virtual instance<> call(instance<> subroutine, instance<SFrame> const& call_frame, std::vector<instance<>> const&) const = 0;
	};

	/******************************************************************************
	** AutoSubroutine
	******************************************************************************/

	template <typename T>
	class AutoSubroutine final
		: public types::Implements<PSubroutine>::For<T>
	{
	public:
		inline virtual instance<SubroutineSignature> signature(instance<> subroutine) const override
		{
			instance<T> ret = subroutine;

			return ret->signature();
		}

		inline virtual instance<SFrame> call_frame(instance<> subroutine) const override
		{
			instance<T> ret = subroutine;

			return ret->call_frame();
		}

		inline virtual instance<> call(instance<> subroutine, instance<SFrame> const& context, std::vector<instance<>> const& args) const override
		{
			instance<T> ret = subroutine;

			return ret->call(context, args);
		}
	};
}}
