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
        
        CRAFT_LISP_EXPORTED virtual types::Function function(instance<>) const;
        CRAFT_LISP_EXPORTED virtual ExpressionStore expression(instance<>) const;
        CRAFT_LISP_EXPORTED virtual instance<> call(instance<>, types::GenericInvoke const&) const;
	};
}}
