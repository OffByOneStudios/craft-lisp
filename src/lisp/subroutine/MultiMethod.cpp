#include "lisp/common.h"
#include "lisp/lisp.h"
#include "MultiMethod.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(MultiMethod)
{
	_.use<PSubroutine>().singleton<AutoSubroutine>();

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance <MultiMethod> mm) -> std::string
		{
			return fmt::format("MultiMethod with {0} subroutines.", mm->subroutineCount());
		});

	_.defaults();
}

MultiMethod::MultiMethod()
{

}


instance<SFrame> MultiMethod::call_frame()
{
	auto ret = instance<Frame>::make();
	ret->setRepresentative(craft_instance());
	return ret;
}

instance<> MultiMethod::call(instance<SFrame> const& frame, std::vector<instance<>> const& args)
{
	auto env = frame->getNamespace()->environment();

	Dispatch _dispatch;
	dispatch(env, type_of(env, args), &_dispatch);

	if (!_dispatch.subroutine)
		throw stdext::exception("Dispatch failed.");

	auto provider = _dispatch.subroutine.getFeature<PSubroutine>();
	auto call_frame = provider->call_frame(_dispatch.subroutine);
	Execution::execute(call_frame);
	return _dispatch.subroutine.getFeature<PSubroutine>()->call(_dispatch.subroutine, call_frame, args);
}

size_t MultiMethod::subroutineCount()
{
	return _subMethods.size();
}

instance<SubroutineSignature> MultiMethod::signature()
{
	return _signature;
}

std::vector<instance<SubroutineSignature>> MultiMethod::signatures()
{
	std::vector<instance<SubroutineSignature>> res;
	for (auto& it : _subMethods)
	{
		res.push_back(it.asFeature<PSubroutine>()->signature(it));
	}
	return res;
}

void MultiMethod::dispatch(instance<Environment> env, instance<lisp::types::SType> type, MultiMethod::Dispatch* dispatch)
{
	bool is_subtype = false;
	size_t index = 0;

	while (!is_subtype)
	{
		if (!(index < _dispatchChain.size()))
			return; // Ran out of possibilities.

		is_subtype = types::is_subtype(env, type, _dispatchChain[index].type);
		index += 1;
	}

	dispatch->subroutine = _subMethods[_dispatchChain[index - 1].index];
}
void MultiMethod::attach(instance<Environment> env, instance<> in)
{
	//
	// Checks
	//

	if (!in.hasFeature<PSubroutine>())
		stdext::exception("Attached subroutine must implement PSubroutine.");

	if (in.typeId().isType<MultiMethod>())
		env->log()->warn("Attaching a multimethod as a function; this has serious performance implications.");

	//
	// Insert - Persistent
	//

	_subMethods.push_back(in);
	
	//
	// Insert - Dispatch
	//

	auto psub = in.asFeature<PSubroutine>();
	auto sig = psub->signature(psub);

	// TODO sort
	
	_dispatchChain.push_back({ sig->asType(), _subMethods.size() - 1 });

	//
	// Insert - Signature
	//
	
}