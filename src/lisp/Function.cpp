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

Function::Function()
{

}

instance<> Function::call(instance<SScope> const& scope, std::vector<instance<>> const& args)
{
	// TODO bind args into scope using binding sexpr

	return scope->environment()->eval(scope, _body);
}

void Function::setBody(instance<Sexpr> body)
{
	_body = body;
}

void Function::setBinding(instance<Sexpr> binding)
{
	_binding = binding;
}
