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

		CRAFT_LISP_EXPORTED instance<Binding> getBinding() const;

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
		CRAFT_LISP_EXPORTED Binding(instance<SScope>, instance<Symbol>, size_t index, instance<BindSite>);

		CRAFT_LISP_EXPORTED size_t getIndex() const;
		CRAFT_LISP_EXPORTED instance<SScope> getScope() const;
		CRAFT_LISP_EXPORTED instance<Symbol> getSymbol() const;
		CRAFT_LISP_EXPORTED instance<BindSite> getSite() const;

		CRAFT_LISP_EXPORTED instance<> getMeta(std::string metaKey, types::TypeId type = types::None) const;
		CRAFT_LISP_EXPORTED void addMeta(std::string metaKey, instance<>);
	};

	/******************************************************************************
	** NamespaceManipulation
	******************************************************************************/

	/*
	Represents the modification of the behavior of the define special form
	*/
	class NamespaceManipulation
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::NamespaceManipulation);
	public:
		enum class Manipulation
		{
			None = 0,
			Namespace,
			Require,
			Import,
			Using,
			Include,
			Load
		};

	private:

		Manipulation _mode;
		std::string _primary;
		std::string _as;

		// TODO symbol renaming/require/exclude rules

	public:
		CRAFT_LISP_EXPORTED NamespaceManipulation();

		static CRAFT_LISP_EXPORTED instance<NamespaceManipulation> SetNamespace(std::string const& namespace_name);
		static CRAFT_LISP_EXPORTED instance<NamespaceManipulation> RequireNamespace(std::string const& namespace_name, std::string const& as = "");
		static CRAFT_LISP_EXPORTED instance<NamespaceManipulation> ImportNamespace(std::string const& uri, std::string const& as = "");
		static CRAFT_LISP_EXPORTED instance<NamespaceManipulation> UsingNamespace(std::string const& uri, std::string const& as = "");
		static CRAFT_LISP_EXPORTED instance<NamespaceManipulation> IncludeNamespace(std::string const& uri, std::string const& as = "");
		static CRAFT_LISP_EXPORTED instance<NamespaceManipulation> LoadNamespace(std::string const& uri, std::string const& as = "");

		CRAFT_LISP_EXPORTED Manipulation getManipulationKind() const;
		CRAFT_LISP_EXPORTED std::string getModuleUri() const;
		CRAFT_LISP_EXPORTED std::string getNamespaceName() const;
		CRAFT_LISP_EXPORTED std::string getTargetName() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
	};

	/******************************************************************************
	** SBindable
	******************************************************************************/

	/*
		Represents a semantic node that is attached to a scope.
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

	protected:
		typedef SymbolTableIndexed<instance<Binding>> _SimpleSymbolTableBindings;

		static inline instance<Binding> _simple_lookup(_SimpleSymbolTableBindings const& ref, instance<Symbol> symbol)
		{
			return ref.lookup(symbol);
		}
		static inline instance<Binding> _simple_define(instance<SScope> this_, _SimpleSymbolTableBindings& ref, instance<Symbol> symbol, instance<BindSite> bindsite)
		{
			return ref.define(symbol, [&](size_t index) { return instance<Binding>::make(this_, symbol, index, bindsite); });
		}

	public:
		CRAFT_LISP_EXPORTED virtual instance<CultSemantics> getSemantics() const;
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const = 0;

		CRAFT_LISP_EXPORTED virtual size_t getSlotCount() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const = 0;
		CRAFT_LISP_EXPORTED virtual instance<Binding> lookupSlot(size_t) const = 0;
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

			return (instance<SScope>)cur;
		}
	};
}}
