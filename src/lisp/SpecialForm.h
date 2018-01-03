#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/*
		This class allows for a generic approach to special forms.

		This will only work for the bootstrapper probably.
	*/
	class SpecialForm
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::SpecialForm)
	public:
		typedef std::function<instance<>(instance<SScope> scope, instance<Sexpr> sexpr)> f_eval;

	private:

		f_eval _eval;

	public:

		CRAFT_LISP_EXPORTED SpecialForm(f_eval);

		CRAFT_LISP_EXPORTED instance<> eval(instance<SScope> scope, instance<Sexpr> sexpr);
	};

}}
