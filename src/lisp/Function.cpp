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

instance<SubroutineSignature> Function::signature()
{
	return _signature;
}

instance<> Function::call(instance<SScope> const& scope, std::vector<instance<>> const& args)
{
	auto call_scope = _signature->eval_frame(scope, args);

	return call_scope->environment()->eval(call_scope, _body);
}

void Function::setBody(instance<Sexpr> body)
{
	_body = body;
}

void Function::setSignature(instance<SubroutineSignature> signature)
{
	_signature = signature;
}
