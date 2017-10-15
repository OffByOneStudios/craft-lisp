#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Symbol
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Symbol)
	public:

		CRAFT_LISP_EXPORTED Symbol(std::string const& s);

		std::string value;
	};

}}