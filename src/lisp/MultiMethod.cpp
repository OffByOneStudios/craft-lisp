#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/MultiMethod.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(MultiMethod)
{
	_.use<PSubroutine>().singleton<AutoSubroutine>();

	_.defaults();
}

MultiMethod::MultiMethod()
{

}

instance<> MultiMethod::call(instance<Scope> const& scope, std::vector<instance<>> const& args)
{
	auto env = scope->environment();
	auto subroutine = dispatch(env, type_of(env, args));

	if (!subroutine)
		throw stdext::exception("Dispatch failed.");

	return subroutine.getFeature<PSubroutine>()->call(subroutine, scope, args);
}

instance<> MultiMethod::dispatch(instance<Environment> env, instance<lisp::types::SType> type)
{
	// TODO implement dispatch and remove this line:
	return _subMethods[0];

	bool is_subtype = false;
	size_t index = 0;

	while (!is_subtype)
	{
		if (!(index < _dispatchChain.size()))
			return instance<>(); // Ran out of possibilities.

		lisp::types::AlgorithmSubtype subtype(env, type, _dispatchChain[index].type);
		subtype.execute_subtype();
		is_subtype = subtype.leftIsSubtype;
	}

	return _subMethods[_dispatchChain[index].index];
}
void MultiMethod::attach(instance<Environment> env, instance<> in)
{
	if (!in.hasFeature<PSubroutine>())
		stdext::exception("Attached subroutine must implement PSubroutine.");

	if (in.typeId().isType<MultiMethod>())
		env->log()->warn("Attaching a multimethod as a function.");

	_subMethods.push_back(in);
	//_dispatchChain.push_back(in);
}