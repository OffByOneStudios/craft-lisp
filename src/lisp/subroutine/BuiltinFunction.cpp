#include "lisp/common.h"
#include "lisp/lisp.h"
#include "BuiltinFunction.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(BuiltinFunction)
{
	_.use<PSubroutine>().singleton<AutoSubroutine>();

	_.defaults();
}

BuiltinFunction::BuiltinFunction(f_call call)
{
	_call = call;
	_signature = SubroutineSignature::makeCollectArgs();
}

BuiltinFunction::BuiltinFunction(instance<SubroutineSignature> sig, f_call call)
{
	_signature = sig;
	_call = call;
}

instance<SubroutineSignature> BuiltinFunction::signature()
{
	return _signature;
}
instance<SFrame> BuiltinFunction::call_frame()
{
	auto ret = instance<Frame>::make();
	ret->setRepresentative(craft_instance_from_this());
	return ret;
}
instance<> BuiltinFunction::call(instance<SFrame> const& frame, std::vector<instance<>> const& args)
{
	return _call(frame, args);
}

void BuiltinFunction::setSignature(instance<SubroutineSignature> signature)
{
	_signature = signature;
}
