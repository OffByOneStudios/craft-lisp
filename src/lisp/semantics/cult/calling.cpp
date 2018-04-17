#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** CallSite
******************************************************************************/

CRAFT_DEFINE(CallSite)
{
	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

CallSite::CallSite(instance<SCultSemanticNode> callee, std::vector<instance<SCultSemanticNode>> args)
{
	_callee = _ast(craft_instance(), callee);

	_args.reserve(args.size());
	for (auto arg : args)
		_args.push_back(_ast(craft_instance(), arg));
}

instance<> CallSite::calleeAst() const
{
	return _callee;
}

size_t CallSite::argCount() const
{
	return _args.size();
}
instance<> CallSite::argAst(size_t index) const
{
	return _args.at(index);
}

instance<SCultSemanticNode> CallSite::getParent() const
{
	return _parent;
}
void CallSite::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

/******************************************************************************
** Function
******************************************************************************/

using lisp::Function;

CRAFT_DEFINE(lisp::Function)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();
	_.use<SScope>().byCasting();

	_.defaults();
}

lisp::Function::Function()
{
}

instance<SCultSemanticNode> lisp::Function::getParent() const
{
	return _parent;
}
void lisp::Function::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

instance<Binding> lisp::Function::getBinding() const
{
	return _binding;
}
void lisp::Function::setBinding(instance<Binding> binding)
{
	_binding = binding;
}

instance<SScope> lisp::Function::getParentScope() const
{
	return _parentSymbols;
}

instance<Binding> lisp::Function::lookup(instance<Symbol> symbol) const
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return _bindings[it->second];
}
instance<Binding> lisp::Function::define(instance<Symbol> symbol, instance<BindSite> ast)
{
	auto key = symbol->symbolStoreId;
	auto lb = _symbolTable.lower_bound(key);

	if (lb != _symbolTable.end() && !(_symbolTable.key_comp()(key, lb->first)))
		throw stdext::exception("Symbol already defined.");

	auto res = instance<Binding>::make(craft_instance(), symbol, ast);
	_bindings.push_back(res);
	auto index = _bindings.size() - 1;
	res->setIndex(index);
	_symbolTable.insert(lb, { key, index });
	return res;
}

/******************************************************************************
** MultiMethod
******************************************************************************/

CRAFT_DEFINE(MultiMethod)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();

	_.defaults();
}

MultiMethod::MultiMethod()
{
}

instance<SCultSemanticNode> MultiMethod::getParent() const
{
	return _parent;
}
void MultiMethod::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

void MultiMethod::attach(instance<BindSite> binding)
{
	auto value = binding->valueAst();

	if (value.isType<Constant>())
		value = value.asType<Constant>()->getValue();

	if (!value.hasFeature<PSubroutine>())
		throw stdext::exception("Bindsite value is not a PSubroutine.");

	auto psub = value.getFeature<PSubroutine>();
	
	auto it = _entries.insert({ psub->function(value), value, psub });
	_dispatcher.add(psub->expression(value), &*it);
}

instance<>  MultiMethod::call_internal(types::GenericInvoke const& invoke) const
{
	std::vector<TypeId> exprs;
	exprs.reserve(invoke.args.size());
	std::transform(invoke.args.begin(), invoke.args.end(), std::back_inserter(exprs),
		[](instance<> const& inst) { return inst.typeId(); });

	auto res = _dispatcher.dispatchWithRecord(exprs);
	auto entry = (_Entry*)std::get<0>(res);

	if (entry == nullptr)
	{
		std::string dispatchList = stdext::join<char, std::vector<TypeId>::iterator>(
			std::string(", "), exprs.begin(), exprs.end(),
			[](auto it) { return it->toString(); });
		throw stdext::exception("Dispatch failed for [{0}].", dispatchList);
	}

	return types::invoke(*std::get<1>(res), entry->function, invoke);
}

instance<Binding> MultiMethod::getBinding() const
{
	return _binding;
}
void MultiMethod::setBinding(instance<Binding> binding)
{
	_binding = binding;
}
