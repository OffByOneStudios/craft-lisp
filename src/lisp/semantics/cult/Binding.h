#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	class SScope;
	class SBindable;

	/******************************************************************************
	** BindSite
	******************************************************************************/

	/*
	Represents the edge between a symbol, the scope it was defined in, and the AST inside it.
	*/
	class BindSite
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BindSite);
	private:
		instance<SCultSemanticNode> _parent;

	public:
		instance<SCultSemanticNode> _bindSymbol;
		instance<SCultSemanticNode> _bindValue;

	public:
		CRAFT_LISP_EXPORTED BindSite();

		CRAFT_LISP_EXPORTED instance<> symbolAst() const;
		CRAFT_LISP_EXPORTED instance<> valueAst() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;
	};

	/******************************************************************************
	** Binding
	******************************************************************************/

	/*
		Represents the edge between a symbol, the scope it was defined in, and the AST inside it.
	*/
	class Binding
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Binding);
	private:
		instance<SScope> _scope;
		instance<Symbol> _symbol;
		instance<BindSite> _site;

		std::map<std::string, instance<>> _meta;

	public:
		CRAFT_LISP_EXPORTED Binding(instance<SScope>, instance<Symbol>, instance<BindSite>);

		CRAFT_LISP_EXPORTED instance<SScope> getScope() const;
		CRAFT_LISP_EXPORTED instance<Symbol> getSymbol() const;
		CRAFT_LISP_EXPORTED instance<BindSite> getSite() const;

		CRAFT_LISP_EXPORTED instance<> getMeta(std::string metaKey, types::TypeId type = types::None) const;
		CRAFT_LISP_EXPORTED void addMeta(std::string metaKey, instance<>);
	};

	/******************************************************************************
	** SBindable
	******************************************************************************/

	/*
	Represents a semantic node that provides a scope.
	*/
	class SBindable
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::SBindable, "lisp.bindable", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const = 0;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) const = 0;
	};

	/******************************************************************************
	** SScope
	******************************************************************************/

	/*
		Represents a semantic node that provides a scope.
	*/
	class SScope
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::SScope, "lisp.scope", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<CultSemantics> getSemantics() const;
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const = 0;

		// E.g. may enclose over other higher scopes
		CRAFT_LISP_EXPORTED virtual bool isLexicalScope() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) = 0;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<> ast) = 0;
	};
}}
