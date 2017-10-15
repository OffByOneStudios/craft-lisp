#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Function
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Function);
	private:

		instance<Sexpr> _body;

	public:
		CRAFT_LISP_EXPORTED Function(instance<Sexpr> body);

		CRAFT_LISP_EXPORTED instance<> call(instance<Scope> const& scope, std::vector<instance<>> const&);
	};

}}
