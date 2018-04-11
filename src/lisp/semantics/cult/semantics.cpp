#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/semantics.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** CultSemantics
******************************************************************************/

CRAFT_DEFINE(CultSemantics)
{
	_.use<PSemantics>().singleton<CultSemanticsProvider>();

	_.defaults();
}


CultSemantics::CultSemantics(instance<lisp::Module> forModule)
{
	_module = forModule;
}

instance<> CultSemantics::read_cultLisp(instance<SScope> scope, instance<> syntax)
{
	if (syntax.typeId().isType<Symbol>())
		return scope->lookup(syntax.asType<Symbol>()->getValue());
	else if (syntax.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = syntax;

		if (expr->cells.size() == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Evaluate Head --
		instance<> head = expr->cells[0];

		head = read_cultLisp(scope, head);

		instance<> inspect_head = head;
		if (inspect_head.typeId().isType<Binding>())
		{
			auto new_inspect_head = inspect_head.asType<Binding>()->getValue(instance<>());
			if (!new_inspect_head)
				new_inspect_head = head.asType<Binding>()->expression();
			inspect_head = new_inspect_head;
		}

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

void CultSemantics::read(instance<CultLispSyntax> syntax, PSemantics::ReadOptions const* opts)
{
	// TODO, make this executed by the interpreter with some special understanding about accessing
	//  macros and a different set of special forms
	// TODO, make this execute on one node at a time (e.g. to prevent blowing the stack) should
	//  also allow it to be re-entrant

	for (auto syntax : syntax->getRootForms())
	{
		auto ret = read_cultLisp(craft_instance(), syntax);

		_ast.push_back(ret);
	}
}


instance<CultSemantics> CultSemantics::getSemantics() const
{
	return craft_instance();
}
instance<SScope> CultSemantics::getParentScope() const
{
	return instance<>();
}

// E.g. may enclose over other higher scopes
bool CultSemantics::isLexicalScope() const
{
	return false;
}

instance<SBinding> CultSemantics::lookup(instance<Symbol> symbol)
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return it->second;
}
instance<SBinding> CultSemantics::define(instance<Symbol> symbol, instance<> ast)
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return it->second;
}

/******************************************************************************
** CultSemanticsProvider
******************************************************************************/

instance<lisp::Module> CultSemanticsProvider::getModule(instance<> semantics) const
{
	semantics.asType<CultSemantics>()->getModule();
}

std::vector<TypeId> CultSemanticsProvider::readsFrom() const
{
	return{ cpptype<CultLispSyntax>::typeDesc() };
}
instance<> CultSemanticsProvider::read(instance<> syntax_, instance<lisp::Module> into, ReadOptions const* opts) const
{
	auto building = instance<CultSemantics>::make(into);
	
	if (syntax_.typeId().isType<CultLispSyntax>())
	{
		building->read(syntax_.asType<CultLispSyntax>(), opts);
	}
	else
	{
		assert(false && "Unknown syntax.");
	}

	return building;
}

std::vector<TypeId> CultSemanticsProvider::transformsFrom() const
{
	return{};
}
instance<> CultSemanticsProvider::transform(instance<> semantics, instance<lisp::Module> into, instance<> transformationOptions = instance<>()) const
{

}

instance<> CultSemanticsProvider::lookup(instance<> semantics_, std::string const& s) const
{
	instance<CultSemantics> semantics = semantics_;

	if (s == "$init")
	{

	}
	else
	{
		// Symbol table lookup
	}
}

/******************************************************************************
** CultSemantics Helpers
******************************************************************************/

void CultSemantics::builtin_addSpecialForm(std::string const& symbol_name)
{
	define(Symbol::makeSymbol(symbol_name), instance<SpecialForm>::make());
}
void CultSemantics::builtin_specialFormReader(std::string const& symbol_name, CultSemantics::f_specialFormReader reader)
{
	lookup(Symbol::makeSymbol(symbol_name));
}

void CultSemantics::builtin_addMultiMethod(std::string const& symbol_name)
{

}
void CultSemantics::builtin_attachMultiMethod(std::string const& symbol_name, std::tuple<types::ExpressionStore, types::Function> impl)
{

}

void CultSemantics::builtin_eval(std::string const& contents)
{

}

/******************************************************************************
** SpecialForm
******************************************************************************/


CRAFT_DEFINE(SpecialForm)
{
	_.defaults();
}

SpecialForm::SpecialForm()
{

}

instance<> SpecialForm::read(instance<CultSemantics> semantics, instance<SScope> scope, instance<Sexpr> form)
{
	return _read(semantics, scope, form);
}
