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
	Represents a semantic node that provides a scope.
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
}}
