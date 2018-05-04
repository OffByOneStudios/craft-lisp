#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	class SScope;
	class SBindable;
	class Binding;

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
		instance<SCultSemanticNode> _bindSymbol;
		instance<SCultSemanticNode> _bindValue;

		instance<Binding> _boundTo;

	public:
		CRAFT_LISP_EXPORTED BindSite(instance<SCultSemanticNode> symbol, instance<SCultSemanticNode> value);
		CRAFT_LISP_EXPORTED BindSite(instance<Symbol> symbol, instance<SCultSemanticNode> value);

		CRAFT_LISP_EXPORTED void craft_setupInstance();

		CRAFT_LISP_EXPORTED bool isDynamicBind() const;
		CRAFT_LISP_EXPORTED bool isAttachSite() const;

		CRAFT_LISP_EXPORTED instance<> symbolAst() const;
		CRAFT_LISP_EXPORTED instance<> valueAst() const;

		inline instance<Symbol> getStaticSymbol() const
		{
			return symbolAst().asType<Constant>()->getValue().asType<Symbol>();
		}

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
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
		size_t _index;

		instance<SScope> _scope;
		instance<Symbol> _symbol;
		instance<BindSite> _site;

		std::map<std::string, instance<>> _meta;

	public:
		CRAFT_LISP_EXPORTED Binding(instance<SScope>, instance<Symbol>, instance<BindSite>);

		CRAFT_LISP_EXPORTED size_t getIndex() const;
		CRAFT_LISP_EXPORTED instance<SScope> getScope() const;
		CRAFT_LISP_EXPORTED instance<Symbol> getSymbol() const;
		CRAFT_LISP_EXPORTED instance<BindSite> getSite() const;

		CRAFT_LISP_EXPORTED void setIndex(size_t);

		CRAFT_LISP_EXPORTED instance<> getMeta(std::string metaKey, types::TypeId type = types::None) const;
		CRAFT_LISP_EXPORTED void addMeta(std::string metaKey, instance<>);
	};

	/******************************************************************************
	** Import
	******************************************************************************/

	/*
	Represents the edge between a symbol, the scope it was defined in, and the AST inside it.
	*/
	class Import
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Import);

	private:
		std::string _importUri;

	public:
		CRAFT_LISP_EXPORTED Import(std::string uri);

		CRAFT_LISP_EXPORTED std::string getUri() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
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
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) = 0;

		CRAFT_LISP_EXPORTED virtual void attach(instance<BindSite>);
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

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const = 0;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<BindSite> ast) = 0;

		inline instance<Binding> lookup_recurse(instance<Symbol> sym)
		{
			instance<Binding> bindRet = lookup(sym);
			if (bindRet) return bindRet;

			instance<SScope> scope = getParentScope();
			while (!bindRet && scope)
			{
				bindRet = scope->lookup(sym);
				if (bindRet) return bindRet;

				scope = scope->getParentScope();
			}

			return instance<>();
		}

		inline static instance<SScope> findScope(instance<SCultSemanticNode> node)
		{
			auto cur = node;
			while (cur)
			{
				if (cur.hasFeature<SScope>())
					break;
				cur = cur->getParent();
			}

			return cur;
		}
	};
}}
