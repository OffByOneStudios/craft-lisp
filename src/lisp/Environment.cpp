#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Environment.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Environment)
{
	_.defaults();
}

Environment::Environment(std::shared_ptr<spdlog::logger> logger)
{
	_logger = logger;

	global = make_library_globals(craft_instance_from_this());
}

std::shared_ptr<spdlog::logger> Environment::log()
{
	return _logger;
}

instance<Sexpr> Environment::read(std::string const& text)
{
	return parse_lisp(text);
}

instance<> Environment::eval(instance<> cell, instance<Scope> scope)
{
	if (cell.typeId().isType<Symbol>())
		return scope->lookup(cell.asType<Symbol>()->value);
	else if (cell.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = cell;

		if (expr->cells.size() == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Evaluate Head --
		instance<> head = expr->car();

		head = eval(head, scope);

		// -- Macro Expand --
		bool was_macro_expanded = false;

		if (head.typeId().isType<Macro>())
		{
			head = head.asType<Macro>()->expand(scope, expr->cells);
			was_macro_expanded = true;
		}

		// Evaluate expansion
		if (was_macro_expanded)
			return eval(expr, scope);

		// -- Evaluate Arguments --
		if (!head.hasFeature<PSubroutine>())
		{
			throw stdext::exception("Cannot call `{0}`", head.toString());
		}

		std::vector<instance<>> sub_expr_values;
		sub_expr_values.reserve(expr->cells.size() - 1);

		for (auto it = expr->cells.begin() + 1, end = expr->cells.end(); it != end; ++it)
		{
			sub_expr_values.push_back(eval(*it, scope));
		}

		// -- Call --
		return head.getFeature<PSubroutine>()->call(head, scope, sub_expr_values);
	}
	else
		return cell;
}