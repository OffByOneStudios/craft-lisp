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
		CRAFT_LISP_EXPORTED virtual instance<> init(instance<Namespace>) const = 0;

		CRAFT_LISP_EXPORTED virtual instance<> addModule(instance<> backend_ns, instance<Module>) const = 0;
		CRAFT_LISP_EXPORTED virtual instance<> addFunction(instance<> backend_module, instance<>) const = 0;

		CRAFT_LISP_EXPORTED virtual instance<> exec(instance<lisp::SFrame> frame, instance<> code) const = 0;
	};

}}
