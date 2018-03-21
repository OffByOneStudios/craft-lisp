#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Keyword
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Keyword)
	public:

		CRAFT_LISP_EXPORTED Keyword(std::string const& s);

		std::string value;
	};

}}