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
		instance<SCultSemanticNode> _callee;

		std::vector<instance<SCultSemanticNode>> _args;

	public:
		CRAFT_LISP_EXPORTED CallSite(instance<SCultSemanticNode> callee, std::vector<instance<SCultSemanticNode>> args);

		CRAFT_LISP_EXPORTED void craft_setupInstance();

		CRAFT_LISP_EXPORTED instance<> calleeAst() const;

		CRAFT_LISP_EXPORTED size_t argCount() const;
		CRAFT_LISP_EXPORTED instance<> argAst(size_t index) const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
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

		friend class FunctionSubroutineProvider;
	private:
		instance<Binding> _binding;

		instance<SScope> _parentSymbols;

		// TODO: Symbol equality
		std::vector<instance<Binding>> _bindings;
		std::map<size_t, size_t> _symbolTable; // Internal table

	public:


	public:
		CRAFT_LISP_EXPORTED Function();

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

		friend class MultiMethodSubroutineProvider;
	private:
		instance<SCultSemanticNode> _parent;
		instance<Binding> _binding;

	private:
        struct _Entry
        {
			types::Function function;
            instance<> subroutine;
			PSubroutine* provider;
        };
        
		plf::colony<_Entry> _entries;
		types::ExpressionDispatcher _dispatcher;
        
	public:
		CRAFT_LISP_EXPORTED MultiMethod();
        
		// A helper to call this multimethod without the normal stackframes...
		//   could cause some weird stuff if it dispatches to a non-c function
		CRAFT_LISP_EXPORTED instance<> call_internal(types::GenericInvoke const& invoke) const;

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) override;

		CRAFT_LISP_EXPORTED virtual void attach(instance<BindSite>) override;
	};
}}
