#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** SCultType
	******************************************************************************/

	/*
		Represents a semantic node that describes a type
	*/
	class SCultType
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::SCultType, "lisp.cult.type", types::FactoryAspectManager);

	public:
	};


	/******************************************************************************
	** CultTypeExpressionHost
	******************************************************************************/

	/*
		Multimethod Definition
	*/
	class CultTypeExpressionHost
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SBindable>
		, public craft::types::Implements<SCultType>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CultTypeExpressionHost);
	private:
		instance<SCultSemanticNode> _parent;
		instance<Binding> _binding;

	private:
		types::ExpressionStore _expression;
        
	public:
		CRAFT_LISP_EXPORTED CultTypeExpressionHost(types::ExpressionStore);

		CRAFT_LISP_EXPORTED bool isTypeComplete();
		CRAFT_LISP_EXPORTED bool isTypeConcrete();

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) override;
	};


	/******************************************************************************
	** TypeDescription
	******************************************************************************/

	/*
		A description of a type.
	*/
	class TypeDescription
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SBindable>
		, public craft::types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::TypeDescription);
	private:
		instance<Binding> _binding;

		instance<SScope> _parentScope;
		_SimpleSymbolTableBindings _symbols;

	private:
		std::vector<instance<SCultSemanticNode>> _statements;

	public:
		CRAFT_LISP_EXPORTED TypeDescription();

		CRAFT_LISP_EXPORTED void preSize(size_t);
		CRAFT_LISP_EXPORTED void push(instance<SCultSemanticNode>);

		CRAFT_LISP_EXPORTED size_t statementCount() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> statementAst(size_t index) const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) override;

		// SScope
	public:
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const override;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<BindSite> ast) override;
	};

}}
