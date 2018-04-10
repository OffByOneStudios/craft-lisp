#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{
	/* Assumes type is also PBackend
	*/

	class PExecutor
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PExecutor, "lisp.executor", types::SingletonProviderManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<> exec(instance<> backend, instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const&) const = 0;

		// The backend must be at the top of the stack.
		CRAFT_LISP_EXPORTED virtual void resume(instance<> backend) const = 0;
	};
}}
