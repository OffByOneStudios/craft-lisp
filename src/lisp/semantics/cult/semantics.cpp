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

instance<SCultSemanticNode> CultSemantics::getParent() const
{
	return instance<>();
}
void CultSemantics::setParent(instance<SCultSemanticNode> parent)
{
	throw parent_already_set_error(craft_instance());
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

instance<Binding> CultSemantics::lookup(instance<Symbol> symbol) const
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return it->second;
}
instance<Binding> CultSemantics::define(instance<Symbol> symbol, instance<BindSite> ast)
{
	auto key = symbol->symbolStoreId;
	auto lb = _symbolTable.lower_bound(key);

	if (lb != _symbolTable.end() && !(_symbolTable.key_comp()(key, lb->first)))
		throw stdext::exception("Symbol already defined.");

	auto res = instance<Binding>::make(craft_instance(), symbol, ast);
	_symbolTable.insert(lb, { key, res });
	return res;
}

std::vector<instance<Binding>> CultSemantics::search(std::string const & search) const
{
	auto const size = search.size();
	auto const& symbols = _module->getNamespace()->symbolStore;

	std::vector<instance<Binding>> res;
	for (auto& it : _symbolTable)
	{
		auto const& sym = symbols.getValue(it.first);

		if (size <= sym.size() && search == sym.substr(0, size))
			res.push_back(it.second);
	}
	return res;
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
	auto symbol = Symbol::makeSymbol(symbol_name);
	auto bindsite = instance<BindSite>::make(symbol, instance<SpecialForm>::make());
	_ast.push_back(bindsite);
	define(symbol, bindsite);
}
void CultSemantics::builtin_specialFormReader(std::string const& symbol_name, CultSemantics::f_specialFormReader reader)
{
	auto binding = lookup(Symbol::makeSymbol(symbol_name));
	auto value = binding->getSite()->valueAst().asType<SpecialForm>()->_read = reader;
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
	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

SpecialForm::SpecialForm()
{

}

instance<SCultSemanticNode> SpecialForm::getParent() const
{
	return _parent;
}
void SpecialForm::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}
