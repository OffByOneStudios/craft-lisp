#include "lisp/common.h"
#include "lisp/lisp.h"
#include "Closure.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Closure)
{
	_.use<PSubroutine>().singleton<AutoSubroutine>();

	_.defaults();
}

Closure::Closure(instance<SFrame> frame, instance<> func)
{
	if (!func.hasFeature<PSubroutine>())
		throw stdext::exception("Can only close over subroutines, {0} is not a subroutine.", func.toString());

	_provider = func.getFeature<PSubroutine>();
	_subroutine = func;
	_frame = frame;
}

instance<> Closure::subroutine()
{
	return _subroutine;
}
instance<SFrame> Closure::closure()
{
	return _frame;
}

instance<SubroutineSignature> Closure::signature()
{
	return _provider->signature(_subroutine);
}

instance<SFrame> Closure::call_frame(instance<SFrame> parent)
{
	auto ret = _provider->call_frame(_subroutine, parent);
	if (ret.typeId().isType<Frame>())
	{
		ret.asType<Frame>()->setLexicalParent(_frame);
	}
	else
		throw stdext::exception("Can't close over frame shape {0}.", ret.toString());

	return ret;
}

instance<> Closure::call(instance<SFrame> const& frame, std::vector<instance<>> const& args)
{
	return _provider->call(_subroutine, frame, args);
}
