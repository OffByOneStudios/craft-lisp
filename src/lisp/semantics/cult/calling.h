#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** CallSite
	******************************************************************************/

	/*
		Callsite node
	*/
	class CallSite
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CallSite);
	private:
		instance<SCultSemanticNode> _parent;

	public:

		instance<> _callee;

		std::vector<instance<>> _args;

	public:
		CRAFT_LISP_EXPORTED CallSite();

		CRAFT_LISP_EXPORTED instance<> calleeAst() const;

		CRAFT_LISP_EXPORTED size_t argCount() const;
		CRAFT_LISP_EXPORTED instance<> argAst(size_t index) const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;
	};

	/******************************************************************************
	** Function
	******************************************************************************/

	/*
		Function Definition
	*/
	class Function
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SBindable>
		, public craft::types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Function);
	private:
		instance<SCultSemanticNode> _parent;

		instance<Binding> _binding;

		// TODO: Symbol equality
		std::map<size_t, instance<Binding>> _symbolTable; // Internal table

	public:


	public:
		CRAFT_LISP_EXPORTED Function();

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) const override;

		// SScope
	public:
		CRAFT_LISP_EXPORTED virtual instance<CultSemantics> getSemantics() const override;
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const override;

		// E.g. may enclose over other higher scopes
		CRAFT_LISP_EXPORTED virtual bool isLexicalScope() const override;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<BindSite> ast) override;
	};

	/******************************************************************************
	** MultiMethod
	******************************************************************************/

	/*
		Multimethod Definition
	*/
	class MultiMethod
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SBindable>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::MultiMethod);
	private:
		instance<SCultSemanticNode> _parent;

		instance<Binding> _binding;


	public:
		CRAFT_LISP_EXPORTED MultiMethod();

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) const override;
	};
}}
