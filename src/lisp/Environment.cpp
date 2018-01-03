#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Environment.h"

using namespace craft::lisp;
using namespace craft::lisp::types;


CRAFT_OBJECT_DEFINE(Environment)
{
	_.defaults();
}

Environment::Environment(std::shared_ptr<spdlog::logger> logger)
{
	_logger = logger;

	ns_cult = instance<Namespace>::make(craft_instance_from_this());
	global = make_library_globals(ns_cult);

	ns_user = instance<Namespace>::make(craft_instance_from_this());
	global = make_library_globals(ns_user);
}

std::shared_ptr<spdlog::logger> Environment::log()
{
	return _logger;
}

//
// Parser
//

instance<Sexpr> Environment::read(std::string const& text)
{
	return parse_lisp(text);
}

//
// Interpreter
//

instance<> Environment::eval(instance<> cell, instance<SScope> scope)
{
	if (cell.typeId().isType<Symbol>())
		return scope->lookup(cell.asType<Symbol>()->value)->value();
	else if (cell.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = cell;

		if (expr->cells.size() == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Evaluate Head --
		instance<> head = expr->car();

		head = eval(head, scope);

		// -- Evaluate Special Forms --
		if (head.typeId().isType<SpecialForm>())
			return head.asType<SpecialForm>()->eval(scope, expr);

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

//
// Types
//

bool Environment::type_isChild(instance<> inst, types::TagId parent)
{
	auto type = inst.typeId();
	if (type.isType<types::CraftType>())
		return type_isChild(inst.asType<types::CraftType>()->type, parent);
	if (type.isType<types::AbstractTag>())
		return type_isChild(inst.asType<types::AbstractTag>()->tag, parent);

	return type_isChild(type, parent);
}
bool Environment::type_isChild(TypeId type, types::TagId parent)
{
	if (parent == 0)
		return false;
	if (type == 0)
		return false;

	auto current = stdext::get_with_default(_type_typeParents, type, types::TagId(0));
	return type_isChild(current, parent);
}
bool Environment::type_isChild(types::TagId current, types::TagId parent)
{
	while (current != 0)
	{
		if (current == parent)
			return true;
		current = stdext::get_with_default(_type_tagParents, current, types::TagId(0));
	}

	return false;
}

TagId Environment::type_getParent(instance<> inst)
{
	auto type = inst.typeId();
	if (type.isType<types::CraftType>())
		return type_getParent(inst.asType<types::CraftType>()->type);
	if (type.isType<types::AbstractTag>())
		return type_getParent(inst.asType<types::AbstractTag>()->tag);

	return type_getParent(type);
}
TagId Environment::type_getParent(TypeId type)
{
	return stdext::get_with_default(_type_typeParents, type, types::TagId(0));
}
TagId Environment::type_getParent(TagId tag)
{
	return stdext::get_with_default(_type_tagParents, tag, types::TagId(0));
}

void Environment::type_setParent(instance<> inst, TagId parent)
{
	auto type = inst.typeId();
	if (type.isType<types::CraftType>())
		return type_setParent(inst.asType<types::CraftType>()->type, parent);
	if (type.isType<types::AbstractTag>())
		return type_setParent(inst.asType<types::AbstractTag>()->tag, parent);

	return type_setParent(type, parent);
}
void Environment::type_setParent(TypeId type, TagId parent)
{
	_type_typeParents[type] = parent;
}
void Environment::type_setParent(TagId tag, TagId parent)
{
	_type_tagParents[tag] = parent;
}

std::string type_name(types::TagId tag)
{
	return "";
}