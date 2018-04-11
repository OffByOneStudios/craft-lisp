#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/binding.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(craft::lisp::SBindable) { _.defaults(); }
CRAFT_DEFINE(craft::lisp::SScope) { _.defaults(); }

/******************************************************************************
** BindSite
******************************************************************************/

CRAFT_DEFINE(BindSite)
{
	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

BindSite::BindSite(instance<SCultSemanticNode> symbol, instance<SCultSemanticNode> value)
{
	_bindSymbol = _ast(craft_instance(), symbol);
	_bindValue = _ast(craft_instance(), value);
}

BindSite::BindSite(instance<Symbol> symbol, instance<SCultSemanticNode> value)
	: BindSite(instance<Constant>::make(symbol), value)
{
}

bool BindSite::isDynamicBind() const
{
	return _bindSymbol.isType<Constant>();
}

instance<> BindSite::symbolAst() const
{
	return _bindSymbol;
}
instance<> BindSite::valueAst() const
{
	return _bindValue;
}

instance<SCultSemanticNode> BindSite::getParent() const
{
	return _parent;
}
void BindSite::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

/******************************************************************************
** Binding
******************************************************************************/

CRAFT_DEFINE(Binding)
{
	_.defaults();
}

Binding::Binding(instance<SScope> scope, instance<Symbol> symbol, instance<BindSite> site)
{
	_scope = scope;
	_symbol = symbol;
	_site = site;
}

instance<SScope> Binding::getScope() const
{
	return _scope;
}

instance<Symbol> Binding::getSymbol() const
{
	return _symbol;
}

instance<BindSite> Binding::getSite() const
{
	return _site;
}

void Binding::addMeta(std::string metaKey, instance<> value)
{
	_meta[metaKey] = value;
}

instance<> Binding::getMeta(std::string metaKey, TypeId type) const
{
	auto it = _meta.find(metaKey);

	if (it != _meta.end()
		&& (type == types::None || it->second.typeId() == type))
		return it->second;

	return instance<>();
}
