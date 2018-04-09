#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{
	/*
		Represents a semantic node that provides a scope.
	*/
	class SScope
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::SScope, "lisp.scope", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<CultSemantics> getSemantics() const;
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParent() const = 0;

		// E.g. may enclose over other higher scopes
		CRAFT_LISP_EXPORTED virtual bool isLexicalScope() const = 0;

		CRAFT_LISP_EXPORTED virtual std::vector<instance<SBinding>> search(std::string const&) = 0;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> lookup(std::string const&) = 0;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> define(std::string name, instance<> ast) = 0;
	};

}}
