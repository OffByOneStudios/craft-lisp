#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/BuiltinFunction.h"

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
}

instance<SubroutineSignature> BuiltinFunction::signature()
{
	return _signature;
}
instance<SFrame> BuiltinFunction::call_frame(instance<SFrame> parent)
{
	return parent;
}
instance<> BuiltinFunction::call(instance<SFrame> const& frame, std::vector<instance<>> const& args)
{
	return _call(frame, args);
}

void BuiltinFunction::setSignature(instance<SubroutineSignature> signature)
{
	_signature = signature;
}
