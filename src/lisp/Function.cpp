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

instance<SFrame> Function::call_frame(instance<SFrame> parent)
{
	return _signature->push_frame(parent);
}

instance<> Function::call(instance<SFrame> const& call_frame, std::vector<instance<>> const& args)
{
	_signature->set_frame(call_frame, args);
	return call_frame->environment()->eval(call_frame, _body);
}

void Function::setBody(instance<Sexpr> body)
{
	_body = body;
}

void Function::setSignature(instance<SubroutineSignature> signature)
{
	_signature = signature;
}
