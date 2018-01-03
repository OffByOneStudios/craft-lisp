#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Namespace
		: public virtual craft::types::Object
		, public types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Namespace);
	private:
		instance<Environment> _environment;

		std::map<std::string, instance<Binding>> _lookup;

	public:
		CRAFT_LISP_EXPORTED Namespace(instance<Environment> env);

	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual void define(instance<Binding>) override;
	};

}}
