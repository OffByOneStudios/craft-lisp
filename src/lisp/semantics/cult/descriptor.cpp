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
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<GetValue> that)
	{
		return instance<GetValue>::make(that->getSymbol());
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

GetValue::GetValue(instance<Symbol> symbol)
{
	_symbol = symbol;
}

instance<Symbol> GetValue::getSymbol() const
{
	return _symbol;
}
instance<Binding> GetValue::getBinding() const
{
	return _binding;
}

void GetValue::bind()
{
	_binding = SScope::findScope(_parent)->lookup_recurse(_symbol);
	if (!_binding)
		throw stdext::exception("GetValue {0} bad symbol {1}.", craft_instance(), _symbol->getValue());
}

/******************************************************************************
** Block
******************************************************************************/

CRAFT_DEFINE(Block)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<Block> that)
	{
		auto clone = instance<Block>::make();

		auto count = that->statementCount();
		clone->preSize(count);
		for (auto i = 0; i < count; ++i)
			clone->push(_clone(that->statementAst(i)));

		return clone;
	});

	_.use<SCultSemanticNode>().byCasting();
	_.use<SScope>().byCasting();

	_.defaults();
}


Block::Block()
{
}

void Block::preSize(size_t cap)
{
	_statements.reserve(cap);
}
void Block::push(instance<SCultSemanticNode> s)
{
	_statements.push_back(_ast(s));
}

size_t Block::statementCount() const
{
	return _statements.size();
}
instance<SCultSemanticNode> Block::statementAst(size_t index) const
{
	return _statements[index];
}

void Block::bind()
{
	_parentScope = SScope::findScope(_parent);

	for (auto stmt : _statements)
	{
		stmt->bind();
	}
}

instance<SScope> Block::getParentScope() const
{
	return _parentScope;
}

instance<Binding> Block::lookup(instance<Symbol> symbol) const
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return _bindings[it->second];
}
instance<Binding> Block::define(instance<Symbol> symbol, instance<BindSite> ast)
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