#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

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

		std::map<std::string, instance<Binding>> _lookup;

	public:

		CRAFT_LISP_EXPORTED Scope(instance<Environment> env);
		CRAFT_LISP_EXPORTED Scope(instance<SScope> parent);

		CRAFT_LISP_EXPORTED instance<SScope> parent() const;

	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual void define(instance<Binding>) override;
	};

}}
