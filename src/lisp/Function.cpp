#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Function.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Function)
{
	_.use<PSubroutine>().singleton<AutoSubroutine>();

	_.defaults();
}

Function::Function(instance<Sexpr> body)
{
	_body = body;
}

instance<> Function::call(instance<Scope> const& scope, std::vector<instance<>> const& args)
{
	// TODO bind args into scope using binding sexpr

	return scope->environment()->eval(_body, scope);
}
