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
	_.use<SCultSemanticNode>().byCasting();
	_.use<SScope>().byCasting();

	_.defaults();
}


CultSemantics::CultSemantics(instance<lisp::Module> forModule)
{
	_module = forModule;
}

instance<SCultSemanticNode> CultSemantics::read_cultLisp(ReadState* rs, instance<> syntax)
{
	if (syntax.typeId().isType<Symbol>())
		return instance<GetValue>::make(syntax.asType<Symbol>());
	else if (syntax.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = syntax;
		auto size = expr->cells.size();

		if (size == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Read Time Dispatch --
		instance<> head = expr->cells[0];

		if (head.typeId().isType<Symbol>())
		{
			instance<Binding> binding = lookup_recurse(head.asType<Symbol>()); // TODO: read state lookup of macros?
			if (binding)
			{
				auto siteValue = binding->getSite()->valueAst();

				// Special Form
				if (siteValue.isType<SpecialForm>())
					return siteValue.asType<SpecialForm>()->_read(rs, expr);

				// Macro Form
				if (siteValue.isType<Macro>())
				{
					throw stdext::exception("Macros not finished.");
				//	ast = inspect_head.asType<Macro>()->expand(scope, expr->cells);
				//	return read(scope, ast);
				}
			}
		}

		// -- Call Site --
		return instance<CallSite>::make(read_cultLisp(rs, head), rs->readAll(expr));
	}
	else // Raw Value
		return instance<Constant>::make(syntax);
}

void CultSemantics::readPrepDefaults()
{
	// These are implict, and must be made available by these execution engine.
	importModule(_module->getNamespace()->requireModule("builtin:cult.system"));
	importModule(_module->getNamespace()->requireModule("builtin:cult.core"));
}

void CultSemantics::read(instance<CultLispSyntax> syntax, PSemantics::ReadOptions const* opts)
{
	// TODO, make this executed by the interpreter with some special understanding about accessing
	//  macros and a different set of special forms
	// TODO, make this execute on one node at a time (e.g. to prevent blowing the stack) should
	//  also allow it to be re-entrant

	for (auto syntax : syntax->getRootForms())
	{
		ReadState rs { craft_instance() };

		auto ret = read_cultLisp(&rs, syntax);

		_ast.push_back(_astbind(ret));
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

instance<Binding> CultSemantics::lookup(instance<Symbol> symbol) const
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
	{
		for (auto m : _modules)
		{
			auto sem = m->get<CultSemantics>();
			if (sem)
			{
				auto res = sem->lookup(symbol);
				if (res)
					return res;
			}
		}

		return instance<>();
	}
	return _bindings[it->second];
}
instance<Binding> CultSemantics::define(instance<Symbol> symbol, instance<BindSite> ast)
{
	auto key = symbol->symbolStoreId;
	auto lb = _symbolTable.lower_bound(key);

	if (lb != _symbolTable.end() && !(_symbolTable.key_comp()(key, lb->first)))
		throw stdext::exception("Symbol already defined.");

	auto res = instance<Binding>::make(craft_instance(), symbol, ast);
	_bindings.push_back(res);
	//auto index = _bindings.size() - 1; 
	auto index = _ast.size(); // hack untill we improve the runtime features
	res->setIndex(index);
	_symbolTable.insert(lb, { key, _bindings.size() - 1 });
	return res;
}

std::vector<instance<Binding>> CultSemantics::search(std::string const & search) const
{
	// TODO search symbols using a trie, and then look for those symbols in modules.

	auto const size = search.size();
	auto const& symbols = _module->getNamespace()->symbolStore;

	std::vector<instance<Binding>> res;
	for (auto& it : _symbolTable)
	{
		auto const& sym = symbols.getValue(it.first);

		if (size <= sym.size() && search == sym.substr(0, size))
			res.push_back(_bindings[it.second]);
	}

	for (auto module : _modules)
	{
		auto sem = module->get<CultSemantics>();
		if (sem)
		{
			auto mres = sem->search(search);
			std::copy(mres.begin(), mres.end(), std::back_inserter(res));
		}
	}

	return res;
}

void CultSemantics::importModule(instance<Module> m)
{
	_modules.insert(_modules.begin(), m);
}

size_t CultSemantics::append(instance<CultSemantics> sem)
{
	size_t start = _ast.size();

	for (auto appending_ast : sem->_ast)
	{
		_ast.push_back(_astclonebind(appending_ast));
	}

	return start;
}
std::vector<size_t> CultSemantics::merge(instance<CultSemantics> sem)
{
	return {}; // TODO implement merge
}

/******************************************************************************
** CultSemanticsProvider
******************************************************************************/

instance<lisp::Module> CultSemanticsProvider::getModule(instance<> semantics) const
{
	return semantics.asType<CultSemantics>()->getModule();
}

std::vector<TypeId> CultSemanticsProvider::readsFrom() const
{
	return{ cpptype<CultLispSyntax>::typeDesc() };
}
instance<> CultSemanticsProvider::read(instance<> syntax_, instance<lisp::Module> into, ReadOptions const* opts) const
{
	auto building = instance<CultSemantics>::make(into);
	
	auto loader = into->getLoader();
	auto prepReplacedDefault = loader.getFeature<PModuleLoader>()->prepSemantics(loader, building);
	if (!prepReplacedDefault) building->readPrepDefaults();

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
instance<> CultSemanticsProvider::transform(instance<> semantics, instance<lisp::Module> into, instance<> transformationOptions) const
{
	return instance<>();
}

instance<> CultSemanticsProvider::lookup(instance<> semantics_, std::string const& s) const
{
	instance<CultSemantics> semantics = semantics_;

	return semantics->lookup(Symbol::makeSymbol(s))->getSite()->valueAst();
}

/******************************************************************************
** CultSemantics Helpers
******************************************************************************/

void CultSemantics::builtin_addSpecialForm(std::string const& symbol_name)
{
	auto symbol = Symbol::makeSymbol(symbol_name);
	auto bindsite = instance<BindSite>::make(symbol, instance<SpecialForm>::makeFromPointer(new SpecialForm()));
	_ast.push_back(_astbind(bindsite));
}
void CultSemantics::builtin_specialFormReader(std::string const& symbol_name, CultSemantics::f_specialFormReader reader)
{
	auto binding = lookup(Symbol::makeSymbol(symbol_name));
	auto value = binding->getSite()->valueAst().asType<SpecialForm>()->_read = reader;
}

void CultSemantics::builtin_addMultiMethod(std::string const& symbol_name)
{
	auto symbol = Symbol::makeSymbol(symbol_name);
	auto bindsite = instance<BindSite>::make(symbol, instance<MultiMethod>::make());
	_ast.push_back(_astbind(bindsite));
}
void CultSemantics::builtin_attachMultiMethod(std::string const& symbol_name, std::tuple<types::ExpressionStore, types::Function> impl)
{
	auto symbol = Symbol::makeSymbol(symbol_name);
	auto binding = lookup(symbol);
	if (!binding)
		builtin_addMultiMethod(symbol_name);
	binding = lookup(symbol);

	auto value = binding->getSite()->valueAst();
	if (!value.isType<MultiMethod>())
		throw stdext::exception("Can't attach to not a multimethod.");

	auto multi = value.asType<MultiMethod>();

	auto bindsite = instance<BindSite>::make(symbol, instance<Constant>::make(instance<CppFunctionPointer>::make(std::get<0>(impl), std::get<1>(impl))));
	_ast.push_back(_astbind(bindsite));
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
