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
		instance<Sexpr> _binding;

	public:
		CRAFT_LISP_EXPORTED Function();

		CRAFT_LISP_EXPORTED instance<> call(instance<Scope> const& scope, std::vector<instance<>> const&);

		CRAFT_LISP_EXPORTED void setBody(instance<Sexpr> body);
		CRAFT_LISP_EXPORTED void setBinding(instance<Sexpr> binding);
	};

}}
