#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{
	class PBackend
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PBackend, "lisp.backend", types::SingletonProviderManager);

	public:
		// Creates a new backend for the given namespace
		CRAFT_LISP_EXPORTED virtual instance<> init(instance<Environment>) const = 0;
	};

}}
