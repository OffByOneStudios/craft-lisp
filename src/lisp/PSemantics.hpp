#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{
	/* Assumes type is also PBackend
	*/

	class PSemantics
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PSemantics, "lisp.semantics.semantics", types::SingletonProviderManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<lisp::Module> getModule(instance<> semantics) const = 0;

		//
		// Read
		//
	public:
		struct ReadOptions
		{
			bool no_macros;
		};

		CRAFT_LISP_EXPORTED virtual instance<> read(instance<lisp::Module> module, ReadOptions const* opts) const = 0;

		//
		// Common semantic features
		//
	public:

		// This is an expensive last resort symbol lookup, you must understand the semantics to get more out of this
		// Should try to provide instances with the following features if possible:
		// * SGenericSubroutine
		CRAFT_LISP_EXPORTED virtual instance<> lookup(instance<> semantics, std::string const&) const = 0;
	};
}}
