#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class SScope
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_ASPECT_DECLARE(craft::lisp::SScope, "lisp.scope", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(std::string const&) = 0;
		CRAFT_LISP_EXPORTED virtual void define(instance<Binding>) = 0;
	};

}}
