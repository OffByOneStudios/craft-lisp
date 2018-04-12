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

        CRAFT_LISP_EXPORTED bool isDynamicCall() const;
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

        struct _Entry
        {
            instance<PSubroutine> subroutine;
        };
        
		plf::colony<_Entry> _entries;
		types::ExpressionDispatcher _dispatcher;
        
	public:
		CRAFT_LISP_EXPORTED MultiMethod();

        CRAFT_LISP_EXPORTED void attach(instance<PSubroutine>);
        
		// A helper to call this multimethod without the normal stackframes...
		//   could cause some weird stuff if it dispatches to a non-c function
		CRAFT_LISP_EXPORTED instance<> call_internal(types::GenericInvoke const& invoke) const;

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
