#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/BootstrapInterpreter.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_OBJECT_DEFINE(BootstrapInterpreter)
{
	_.use<PBackend>().singleton<BootstrapInterpreterProvider>();

	_.defaults();
}

BootstrapInterpreter::BootstrapInterpreter(instance<Namespace> lisp)
{
	_lisp = lisp;
}

BootstrapInterpreterProvider::BootstrapInterpreterProvider()
{

}

instance<> BootstrapInterpreterProvider::init(instance<Namespace> ns) const
{
	return instance<BootstrapInterpreter>::make(ns);
}

instance<> BootstrapInterpreterProvider::addModule(instance<> backend_ns, instance<lisp::Module> lisp_module) const
{
	return instance<>();
}

instance<> BootstrapInterpreterProvider::addFunction(instance<> backend_module, instance<>) const
{
	return instance<>();
}

instance<> BootstrapInterpreterProvider::exec(instance<lisp::SFrame> frame, instance<> code) const
{
	if (code.typeId().hasFeature<SBinding>())
		return code.asFeature<SBinding>()->getValue(frame);
	else if (code.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = code;

		if (expr->cells.size() == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Evaluate Head --
		instance<> head = expr->car();

		head = exec(frame, head);

		// -- Evaluate Special Forms --
		if (head.typeId().isType<SpecialForm>())
			return head.asType<SpecialForm>()->eval(frame, expr);

		// -- Evaluate Subroutine --
		if (!head.hasFeature<PSubroutine>())
		{
			throw stdext::exception("Cannot call `{0}`", head.toString());
		}

		auto subroutine_provider = head.getFeature<PSubroutine>();

		std::vector<instance<>> sub_expr_values;
		sub_expr_values.reserve(expr->cells.size() - 1);

		for (auto it = expr->cells.begin() + 1, end = expr->cells.end(); it != end; ++it)
		{
			sub_expr_values.push_back(exec(frame, *it));
		}

		// -- Call --
		frame = subroutine_provider->call_frame(head);
		Execution::execute(frame);
		return subroutine_provider->call(head, frame, sub_expr_values);
	}
	else
		return code;
}
