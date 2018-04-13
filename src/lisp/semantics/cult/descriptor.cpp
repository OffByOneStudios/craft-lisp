#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/descriptor.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** Variable
******************************************************************************/

CRAFT_DEFINE(Variable)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();

	_.defaults();
}

Variable::Variable()
{

}

instance<SCultSemanticNode> Variable::initalizerAst() const
{
	return _initalizer;
}
instance<SCultSemanticNode> Variable::typeAst() const
{
	return _type_ast;
}

instance<SCultSemanticNode> Variable::getParent() const
{
	return _parent;
}
void Variable::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

instance<Binding> Variable::getBinding() const
{
	return _binding;
}
void Variable::setBinding(instance<Binding> binding)
{
	_binding = binding;
}

/******************************************************************************
** GetValue
******************************************************************************/

CRAFT_DEFINE(GetValue)
{
	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

GetValue::GetValue(instance<Binding> binding)
{
	_binding = binding;
}

instance<Binding> GetValue::getBinding() const
{
	return _binding;
}
instance<SCultSemanticNode> GetValue::getParent() const
{
	return _parent;
}
void GetValue::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

/******************************************************************************
** Block
******************************************************************************/

CRAFT_DEFINE(Block)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SScope>().byCasting();

	_.defaults();
}


Block::Block(instance<SScope> parent_scope)
{
	_parentSymbols = parent_scope;
}

void Block::preSize(size_t cap)
{
	_statements.reserve(cap);
}
void Block::push(instance<SCultSemanticNode> s)
{
	_statements.push_back(_ast(craft_instance(), s));
}

size_t Block::statementCount() const
{
	return _statements.size();
}
instance<SCultSemanticNode> Block::statementAst(size_t index) const
{
	return _statements[index];
}

instance<SCultSemanticNode> Block::getParent() const
{
	return _parent;
}
void Block::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

instance<SScope> Block::getParentScope() const
{
	return _parentSymbols;
}

instance<Binding> Block::lookup(instance<Symbol> symbol) const
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return it->second;
}
instance<Binding> Block::define(instance<Symbol> symbol, instance<BindSite> ast)
{
	auto key = symbol->symbolStoreId;
	auto lb = _symbolTable.lower_bound(key);

	if (lb != _symbolTable.end() && !(_symbolTable.key_comp()(key, lb->first)))
		throw stdext::exception("Symbol already defined.");

	auto res = instance<Binding>::make(craft_instance(), symbol, ast);
	_symbolTable.insert(lb, { key, res });
	return res;
}