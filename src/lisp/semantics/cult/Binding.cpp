#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/Binding.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(craft::lisp::SBindable) { _.defaults(); }
CRAFT_DEFINE(craft::lisp::SScope) { _.defaults(); }

void SBindable::attach(instance<BindSite>)
{
	throw stdext::exception("This bindable ({0}) does not support attachement.", craft_featuredInstance().typeId());
}

instance<CultSemantics> SScope::getSemantics() const
{
	instance<SScope> scope = getParentScope();
	while (scope && !scope.isType<CultSemantics>())
		scope = scope->getParentScope();
	return (instance<CultSemantics>)scope;
}

/******************************************************************************
** BindSite
******************************************************************************/

CRAFT_DEFINE(BindSite)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<BindSite> that)
	{
		return instance<BindSite>::make(_clone(that->symbolAst()), _clone(that->valueAst()));
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

BindSite::BindSite(instance<SCultSemanticNode> symbol, instance<SCultSemanticNode> value)
{
	_bindSymbol = symbol;
	_bindValue = value;
}

BindSite::BindSite(instance<Symbol> symbol, instance<SCultSemanticNode> value)
	: BindSite(instance<Constant>::make(symbol), value)
{
}

void BindSite::craft_setupInstance()
{
	Object::craft_setupInstance();

	_ast(_bindSymbol);
	_ast(_bindValue);
}

bool BindSite::isDynamicBind() const
{
	return !_bindSymbol.isType<Constant>();
}

bool BindSite::isAttachSite() const
{
	return _boundTo->getSite() != craft_instance();
}

instance<> BindSite::symbolAst() const
{
	return _bindSymbol;
}
instance<> BindSite::valueAst() const
{
	return _bindValue;
}

instance<Binding> BindSite::getBinding() const
{
	return _boundTo;
}

void BindSite::bind()
{
	_bindSymbol->bind();

	instance<Binding> binding;
	if (!isDynamicBind())
	{
		auto staticSymbol = getStaticSymbol();
		auto bindScope = SScope::findScope(_parent);

		binding = bindScope->lookup(staticSymbol);

		if (!binding)
			binding = bindScope->define(staticSymbol, craft_instance());
		else
		{
			auto binded = binding->getSite()->valueAst();

			if (binded.hasFeature<SBindable>())
				binded.getFeature<SBindable>()->attach(craft_instance());
			else
				throw stdext::exception("{1}> Symbol already defined (target AST {0} does not support attach).", binded, sourceLocationToString());
		}

		_boundTo = binding;
	}
	else
		throw stdext::exception("Not Implemented: Dynamic Bindings");

	_bindValue->bind();

	if (_bindValue.hasFeature<SBindable>())
		_bindValue.getFeature<SBindable>()->setBinding(binding);
}

/******************************************************************************
** Binding
******************************************************************************/

CRAFT_DEFINE(Binding)
{
	_.defaults();
}

Binding::Binding(instance<SScope> scope, instance<Symbol> symbol, size_t index, instance<BindSite> site)
{
	_index = index;
	_scope = scope;
	_symbol = symbol;
	_site = site;
}

size_t Binding::getIndex() const
{
	return _index;
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



/******************************************************************************
** Import
******************************************************************************/

CRAFT_DEFINE(NamespaceManipulation)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<NamespaceManipulation> that)
	{
		switch (that->getManipulationKind())
		{
			case NamespaceManipulation::Manipulation::Namespace: return NamespaceManipulation::SetNamespace(that->getNamespaceName());
			case NamespaceManipulation::Manipulation::Require: return NamespaceManipulation::RequireNamespace(that->getNamespaceName(), that->getTargetName());
			case NamespaceManipulation::Manipulation::Import: return NamespaceManipulation::ImportNamespace(that->getNamespaceName(), that->getTargetName());
			case NamespaceManipulation::Manipulation::Using: return NamespaceManipulation::UsingNamespace(that->getNamespaceName(), that->getTargetName());
			case NamespaceManipulation::Manipulation::Include: return NamespaceManipulation::IncludeNamespace(that->getNamespaceName(), that->getTargetName());
			case NamespaceManipulation::Manipulation::Load: return NamespaceManipulation::LoadNamespace(that->getNamespaceName(), that->getTargetName());
			
			default: throw stdext::exception("Unknown scope manipulation {0}", (int)that->getManipulationKind());
		}
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

NamespaceManipulation::NamespaceManipulation()
{

}

instance<NamespaceManipulation> NamespaceManipulation::SetNamespace(std::string const& namespace_name)
{
	auto ret = instance<NamespaceManipulation>::make();
	ret->_mode = Manipulation::Namespace;
	ret->_primary = namespace_name;
	return ret;
}

instance<NamespaceManipulation> NamespaceManipulation::RequireNamespace(std::string const& uri, std::string const& as)
{
	auto ret = instance<NamespaceManipulation>::make();
	ret->_mode = Manipulation::Require;
	ret->_primary = uri;
	ret->_as = as;
	return ret;
}

instance<NamespaceManipulation> NamespaceManipulation::ImportNamespace(std::string const& namespace_name, std::string const& as)
{
	auto ret = instance<NamespaceManipulation>::make();
	ret->_mode = Manipulation::Import;
	ret->_primary = namespace_name;
	ret->_as = as;
	return ret;
}

instance<NamespaceManipulation> NamespaceManipulation::UsingNamespace(std::string const& namespace_name, std::string const& as)
{
	auto ret = instance<NamespaceManipulation>::make();
	ret->_mode = Manipulation::Using;
	ret->_primary = namespace_name;
	ret->_as = as;
	return ret;
}

instance<NamespaceManipulation> NamespaceManipulation::IncludeNamespace(std::string const& namespace_name, std::string const& as)
{
	auto ret = instance<NamespaceManipulation>::make();
	ret->_mode = Manipulation::Include;
	ret->_primary = namespace_name;
	ret->_as = as;
	return ret;
}

instance<NamespaceManipulation> NamespaceManipulation::LoadNamespace(std::string const& namespace_name, std::string const& as)
{
	auto ret = instance<NamespaceManipulation>::make();
	ret->_mode = Manipulation::Load;
	ret->_primary = namespace_name;
	ret->_as = as;
	return ret;
}



NamespaceManipulation::Manipulation NamespaceManipulation::getManipulationKind() const
{
	return _mode;
}
std::string NamespaceManipulation::getModuleUri() const
{
	assert(_mode == Manipulation::Require);
	return _primary;
}
std::string NamespaceManipulation::getNamespaceName() const
{
	assert(_mode == Manipulation::Namespace || _mode == Manipulation::Import);
	return _primary;
}
std::string NamespaceManipulation::getTargetName() const
{
	return _as;
}

void NamespaceManipulation::bind()
{
	auto parent_scope = SScope::findScope(_parent);
	if (!parent_scope.isType<CultSemantics>())
		throw stdext::exception("{0}> Can only import at module scope.", sourceLocationToString());

	instance<CultSemantics> sem = (instance<CultSemantics>)parent_scope;

	switch (_mode)
	{
		case Manipulation::Import:
		{
			auto required_module = sem->getModule()->getNamespace()->importModule(sem->getModule(), _primary); // TODO store reference somewhere for this node?
			required_module->initialize(); // TODO wait for initalization to complete
			sem->importModule(required_module); // TODO move to runtime of this module
		} break;
		default: throw stdext::exception("Unsupported scope manipulation {0}", (int)_mode);
	}
}
