#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/SpecialForm.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(SpecialForm)
{
	_.defaults();
}

SpecialForm::SpecialForm(f_eval eval)
{
	_eval = eval;
}

instance<> SpecialForm::eval(instance<Scope> scope, instance<Sexpr> sexpr)
{
	return _eval(scope, sexpr);
}