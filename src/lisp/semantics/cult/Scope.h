#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{

	class Scope
		: public virtual craft::types::Object
		, public types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Scope);
	private:

		instance<Environment> _environment;
		instance<SScope> _parent;

		std::map<std::string, instance<SBinding>> _lookup;

	public:

		CRAFT_LISP_EXPORTED Scope(instance<Environment> env);
		CRAFT_LISP_EXPORTED Scope(instance<SScope> parent);

	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Namespace> namespace_() const override;
		CRAFT_LISP_EXPORTED virtual instance<SScope> parent() const override;
		CRAFT_LISP_EXPORTED virtual std::vector<instance<SBinding>> search(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> define(std::string name, instance<> value) override;
	};

}}
