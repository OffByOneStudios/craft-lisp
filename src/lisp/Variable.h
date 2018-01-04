#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Variable
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Variable);
	private:
		instance<> _value;

	public:
		CRAFT_LISP_EXPORTED Variable();
		CRAFT_LISP_EXPORTED Variable(instance<> value);

		CRAFT_LISP_EXPORTED instance<> get();
		CRAFT_LISP_EXPORTED void set(instance<>);
	};

}}
