#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** PClone
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PSubroutine abstract
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_PROVIDER_DECLARE(PSubroutine, "lisp.subroutine", craft::types::SingletonProviderManager);

	public:
		virtual instance<SubroutineSignature> signature(instance<> subroutine) const = 0;

		virtual instance<> call(instance<> subroutine, instance<SScope> const&, std::vector<instance<>> const&) const = 0;
	};

	/******************************************************************************
	** DefaultCopyConstructor
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

		inline virtual instance<> call(instance<> subroutine, instance<SScope> const& scope, std::vector<instance<>> const& args) const override
		{
			instance<T> ret = subroutine;

			return ret->call(scope, args);
		}
	};
}}
