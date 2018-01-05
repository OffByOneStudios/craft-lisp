#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Variable.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_OBJECT_DEFINE(Variable)
{
	_.defaults();
}

Variable::Variable()
{
	_value = instance<>();
}

Variable::Variable(instance<> value)
{
	_value = value;
}

instance<> Variable::get()
{
	return _value;
}

void Variable::set(instance<> value)
{
	_value = value;
}
