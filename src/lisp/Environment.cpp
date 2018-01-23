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

instance<Sexpr> Environment::parse(instance<SScope> scope, std::string const& text)
{
	return parse_lisp(text);
}

instance<> Environment::parse_read(instance<SScope> scope, instance<Sexpr> ast)
{
	auto result = instance<Sexpr>::make();
	for (auto cell : ast->cells)
	{
		result->cells.push_back(this->read(scope, cell));
	}
	return result;
}
instance<> Environment::parse_eval(instance<SScope> scope, instance<Sexpr> ast)
{
	instance<Sexpr> top_level = parse_read(scope, ast);

	auto execution = instance<Execution>::make(craft_instance_from_this(), scope->namespace_());
	auto frame = instance<Frame>::make(execution);

	instance<> result;
	for (auto cell : top_level->cells)
	{
		result = this->eval(frame, cell);
	}
	return result;
}

instance<> Environment::read(instance<SScope> scope, std::string const& text)
{
	auto ast = parse(scope, text);

	return parse_read(scope, ast);
}

instance<> Environment::read(instance<SScope> scope, instance<> ast)
{
	if (ast.typeId().isType<Symbol>())
		return scope->lookup(ast.asType<Symbol>()->value);
	else if (ast.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = ast;

		if (expr->cells.size() == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Evaluate Head --
		instance<> head = expr->car();

		head = read(scope, head);

		instance<> inspect_head = head;
		if (head.typeId().isType<Binding>())
			inspect_head = head.asType<Binding>()->expression();

		// -- Evaluate Special Forms --
		if (inspect_head.typeId().isType<SpecialForm>())
			return inspect_head.asType<SpecialForm>()->read(scope, inspect_head, expr);

		// -- Macro Expand --
		if (inspect_head.typeId().isType<Macro>())
		{
			ast = inspect_head.asType<Macro>()->expand(scope, expr->cells);
			return read(scope, ast);
		}

		return read_rest(scope, head, ast);
	}
	else
		return ast;
}

instance<Sexpr> Environment::read_rest(instance<SScope> scope, instance<> head, instance<Sexpr> ast)
{
	instance<Sexpr> ret = instance<Sexpr>::make();

	ret->cells.push_back(head);
	for (auto cell : ast->cdr())
		ret->cells.push_back(this->read(scope, cell));

	return ret;
}
instance<> Environment::read_eval(instance<SScope> scope, instance<> code)
{
	auto execution = instance<Execution>::make(craft_instance_from_this(), scope->namespace_());
	auto frame = instance<Frame>::make(execution);
	return eval(frame, code);
}

//
// Interpreter
//

instance<> Environment::eval(std::string const& text)
{
	auto exec = instance<Execution>::make(craft_instance_from_this(), ns_user);
	return eval(instance<Frame>::make(exec), text);
}

instance<> Environment::eval(instance<> code)
{
	auto exec = instance<Execution>::make(craft_instance_from_this(), ns_user);
	return eval(instance<Frame>::make(exec), code);
}

instance<> Environment::eval(instance<SFrame> frame, std::string const& text)
{
	instance<SScope> scope = frame->execution()->namespace_();

	auto ast = this->parse(scope, text);

	instance<> last_value;
	for (auto cell : ast->cells)
	{
		auto code = this->read(scope, cell);
		last_value = this->eval(frame, code);
	}
	return last_value;
}

instance<> Environment::eval(instance<SFrame> frame, instance<> code)
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

		head = eval(frame, head);

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
			sub_expr_values.push_back(eval(frame, *it));
		}

		// -- Call --
		frame = subroutine_provider->call_frame(head, frame);
		return subroutine_provider->call(head, frame, sub_expr_values);
	}
	else
		return code;
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