#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cpp/cpp_semantics.h"
#include "lisp/semantics/cpp/CppFunctionPointer.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** CppFunctionPointer
******************************************************************************/

class craft::lisp::CppFunctionPointerSubroutineProvider
	: public Implements<PSubroutine>::For<CppFunctionPointer>
{
	virtual types::Function function(instance<> _) const override
	{
		instance<CppFunctionPointer> cfp = _;

		return cfp->function;
	}
	virtual types::ExpressionStore expression(instance<> _) const override
	{
		instance<CppFunctionPointer> cfp = _;

		return cfp->expression;
	}

	//
	// Performs a full call on the subroutine, including pushing to the current execution
	//
	virtual instance<> execute(instance<> _, types::GenericInvoke const& call) const override
	{
		instance<CppFunctionPointer> cfp = _;

		ExpressionStore expres(new ExpressionArrow(to_expression_tuple(call), &ExpressionBottom::Value));

		if (!isSubtype(expres.root(), cfp->expression.root()))
		{
			stdext::exception("Given invoke does not match function expression.");
		}

		return types::invoke(cfp->expression, cfp->function, call);
	}
};

CRAFT_DEFINE(CppFunctionPointer)
{
	_.use<PSubroutine>().singleton<CppFunctionPointerSubroutineProvider>();

	_.defaults();
}

CppFunctionPointer::CppFunctionPointer(types::ExpressionStore const& es, types::Function const& fn)
	: expression(es), function(fn)
{
}
