#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{
	/* Assumes type is also PBackend
	*/

	class PCompiler
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PCompiler, "lisp.compiler", types::SingletonProviderManager);

	public:
		// Create a new compiler options
		// Should support serialization (not a dependency of this project) and PClone.
		CRAFT_LISP_EXPORTED virtual instance<> makeCompilerOptions() const = 0;

		// Note: should be thread safe
		CRAFT_LISP_EXPORTED virtual void compile(instance<> backend, instance<> options, std::string const& path, instance<lisp::Module> module) const = 0;
	};
}}
