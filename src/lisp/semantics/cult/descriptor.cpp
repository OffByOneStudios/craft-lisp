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
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<lisp::Variable> that)
	{
		auto initAst = that->initalizerAst();
		if (initAst) initAst = _clone(initAst);
		auto typeAst = that->typeAst();
		if (typeAst) typeAst = _clone(typeAst);

		return instance<Variable>::make(initAst, typeAst);
	});

	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();

	_.defaults();
}

Variable::Variable(instance<SCultSemanticNode> initalizer, instance<SCultSemanticNode> type)
{
	_initalizer = initalizer;
	_type = type;
}

void Variable::craft_setupInstance()
{
	Object::craft_setupInstance();

	if (_initalizer) _ast(_initalizer);
	if (_type) _ast(_type);
}

instance<SCultSemanticNode> Variable::initalizerAst() const
{
	return _initalizer;
}
instance<SCultSemanticNode> Variable::typeAst() const
{
	return _type;
}

void Variable::bind()
{
	if (_initalizer) _initalizer->bind();
	if (_type) _type->bind();
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
** Resolve
******************************************************************************/

CRAFT_DEFINE(Resolve)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<Resolve> that)
	{
		return instance<Resolve>::make(that->getSymbol(), that->_mode);
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

Resolve::Resolve(instance<Symbol> symbol, Mode mode)
{
	_symbol = symbol;
	_mode = mode;

	assert(!symbol->isKeyword());

	// '/' and '@' are one "resolution" from resolve's perspective
	// '.' and ':' (non-keyword) seperate resolutions
	auto size = symbol->size();
	std::vector<uint32_t> parts;
	for (auto i = 0; i < size; ++i)
	{
		auto sym = symbol->at(i);
		if (Symbol::isPath(sym) &&
			Symbol::toChar(sym) == '.' || Symbol::toChar(sym) == ':')
		{
			_chain.push_back(Symbol::makeSymbol(parts));
			_ops.push_back(sym);
			parts.clear();
		}
		else
		{
			parts.push_back(sym);
		}
	}
	_chain.push_back(Symbol::makeSymbol(parts));

	assert(_chain.size() > 0);
}

bool Resolve::isGetter()
{
	return _mode == Mode::ResolveAndGet;
}

size_t Resolve::size() const
{
	return _chain.size();
}

instance<Symbol> Resolve::at(size_t at) const
{
	return _chain.at(at);
}
uint32_t Resolve::op(size_t at) const
{
	return _ops.at(at);
}

instance<Symbol> Resolve::getSymbol() const
{
	return _symbol;
}
instance<Binding> Resolve::getBinding() const
{
	return _binding;
}

instance<> Resolve::getConstantValue() const
{
	//_binding
	if (!_binding) throw stdext::exception("Binding is Not Set");
	//valueAst isType
	auto s = _binding->getSite()->valueAst();

	if(!s.isType<Constant>()) throw stdext::exception("Binding is Not Constant");
	return s.asType<Constant>()->getValue();
}

void Resolve::bind()
{
	_binding = SScope::findScope(_parent)->lookup_recurse(_chain[0]);
	if (!_binding)
		throw stdext::exception("{2}> Resolve {0} bad symbol {1}.", craft_instance(), _chain[0]->getDisplay(), sourceLocationToString());
}

/******************************************************************************
** Assign
******************************************************************************/

CRAFT_DEFINE(Assign)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<lisp::Assign> that)
	{
		return instance<Assign>::make(_clone(that->slotAst()), _clone(that->valueAst()));
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

Assign::Assign(instance<SCultSemanticNode> slot, instance<SCultSemanticNode> value)
{
	_slot = slot;
	_value = value;
}

void Assign::craft_setupInstance()
{
	Object::craft_setupInstance();

	_ast(_slot);
	_ast(_value);
}

instance<SCultSemanticNode> Assign::slotAst() const
{
	return _slot;
}
instance<SCultSemanticNode> Assign::valueAst() const
{
	return _value;
}

void Assign::bind()
{
	_slot->bind();
	_value->bind();
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
size_t Block::getSlotCount() const
{
	return _symbols.bindings.size();
}

instance<Binding> Block::lookup(instance<Symbol> symbol) const
{
	return _simple_lookup(_symbols, symbol);
}
instance<Binding> Block::lookupSlot(size_t i) const
{
	return _symbols.bindings.at(i);
}
instance<Binding> Block::define(instance<Symbol> symbol, instance<BindSite> ast)
{
	return _simple_define(craft_instance(), _symbols, symbol, ast);
}