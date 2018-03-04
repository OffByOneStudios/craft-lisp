#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class PBackend
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_PROVIDER_DECLARE(craft::lisp::PBackend, "lisp.backend", types::SingletonProviderManager);

	public:
		CRAFT_LISP_EXPORTED virtual void init(instance<Environment>) const = 0;
	};

}}
