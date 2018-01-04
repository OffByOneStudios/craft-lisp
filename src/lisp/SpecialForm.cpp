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

SpecialForm::SpecialForm(f_read read, f_eval eval)
{
	_read = read;
	_eval = eval;
}

instance<> SpecialForm::read(instance<SScope> scope, instance<> head, instance<Sexpr> sexpr)
{
	return _read(scope, head, sexpr);
}

instance<> SpecialForm::eval(instance<SScope> scope, instance<Sexpr> sexpr)
{
	return _eval(scope, sexpr);
}