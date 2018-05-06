#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/typing.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(SCultType) { _.defaults(); }

/******************************************************************************
** CultTypeExpressionHost
******************************************************************************/

CRAFT_DEFINE(CultTypeExpressionHost)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();
	_.use<SCultType>().byCasting();

	_.defaults();
}

CultTypeExpressionHost::CultTypeExpressionHost(ExpressionStore es)
{
	_expression = es;
}

bool CultTypeExpressionHost::isTypeComplete()
{
	return true;
}
bool CultTypeExpressionHost::isTypeConcrete()
{
	return true;
}

instance<Binding> CultTypeExpressionHost::getBinding() const
{
	return _binding;
}
void CultTypeExpressionHost::setBinding(instance<Binding> binding)
{
	_binding = binding;
}

/******************************************************************************
** TypeDescription
******************************************************************************/

CRAFT_DEFINE(TypeDescription)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<TypeDescription> that)
	{
		auto clone = instance<TypeDescription>::make();

		auto count = that->statementCount();
		clone->preSize(count);
		for (auto i = 0; i < count; ++i)
			clone->push(_clone(that->statementAst(i)));

		return clone;
	});

	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();
	_.use<SScope>().byCasting();

	_.defaults();
}


TypeDescription::TypeDescription()
{
}

void TypeDescription::preSize(size_t cap)
{
	_statements.reserve(cap);
}
void TypeDescription::push(instance<SCultSemanticNode> s)
{
	_statements.push_back(_ast(s));
}

size_t TypeDescription::statementCount() const
{
	return _statements.size();
}
instance<SCultSemanticNode> TypeDescription::statementAst(size_t index) const
{
	return _statements[index];
}

void TypeDescription::bind()
{
	_parentScope = SScope::findScope(_parent);

	for (auto stmt : _statements)
	{
		stmt->bind();
	}
}

instance<Binding> TypeDescription::getBinding() const
{
	return _binding;
}
void TypeDescription::setBinding(instance<Binding> binding)
{
	_binding = binding;
}

instance<SScope> TypeDescription::getParentScope() const
{
	return _parentScope;
}

instance<Binding> TypeDescription::lookup(instance<Symbol> symbol) const
{
	auto it = _symbolTable.find(symbol->symbolStoreId);
	if (it == _symbolTable.end())
		return instance<>();
	return _bindings[it->second];
}
instance<Binding> TypeDescription::define(instance<Symbol> symbol, instance<BindSite> ast)
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
