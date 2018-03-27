#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{
	class Function
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Function);
	private:

		instance<SubroutineSignature> _signature;
		instance<Sexpr> _body;

		std::map<instance<lisp::types::Tuple>, instance<>> _cache;

	public:
		instance<> backend;

	public:
		CRAFT_LISP_EXPORTED Function();

		CRAFT_LISP_EXPORTED instance<SubroutineSignature> signature();
		CRAFT_LISP_EXPORTED instance<SFrame> call_frame();
		CRAFT_LISP_EXPORTED instance<> call(instance<SFrame> const& frame, std::vector<instance<>> const&);

		CRAFT_LISP_EXPORTED void setBody(instance<Sexpr> body);
		CRAFT_LISP_EXPORTED void setSignature(instance<SubroutineSignature> signature);
	};

}}
