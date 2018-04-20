#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/typing.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(SCultType) { _.defaults(); }

/******************************************************************************
** CultTypeExpressionHost
******************************************************************************/

CRAFT_DEFINE(CultTypeExpressionHost)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();
	_.use<SCultType>().byCasting();

	_.defaults();
}

CultTypeExpressionHost::CultTypeExpressionHost(ExpressionStore es)
{
	_expression = es;
}

bool CultTypeExpressionHost::isTypeComplete()
{
	return true;
}
bool CultTypeExpressionHost::isTypeConcrete()
{
	return true;
}

instance<Binding> CultTypeExpressionHost::getBinding() const
{
	return _binding;
}
void CultTypeExpressionHost::setBinding(instance<Binding> binding)
{
	_binding = binding;
}