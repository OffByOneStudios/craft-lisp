#include "lisp/common.h"
#include "lisp/lisp.h"
#include "Function.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Function)
{
	_.use<SSubroutine>().singleton<AutoSubroutine>();

	_.defaults();
}

Function::Function()
{

}

instance<SubroutineSignature> Function::signature()
{
	return _signature;
}

instance<SFrame> Function::call_frame()
{
	return _signature->push_frame();
}

instance<> Function::call(instance<SFrame> const& call_frame, std::vector<instance<>> const& args)
{
	_signature->set_frame(call_frame, args);
	return call_frame->getNamespace()->environment()->eval(call_frame, _body);
}

void Function::setBody(instance<Sexpr> body)
{
	_body = body;
}

void Function::setSignature(instance<SubroutineSignature> signature)
{
	_signature = signature;
}
