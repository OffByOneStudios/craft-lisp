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
		CRAFT_LISP_EXPORTED virtual void exec(instance<> backend, instance<lisp::Module> module, std::string const& entry, GenericCall const&) const = 0;
	};
}}
