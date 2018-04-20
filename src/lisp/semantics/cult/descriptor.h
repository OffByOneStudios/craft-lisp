#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** Variable
	******************************************************************************/

	/*
		Variable define node
	*/
	class Variable
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SBindable>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Variable);
	private:
		instance<Binding> _binding;

	private:

		instance<SCultSemanticNode> _initalizer;
		instance<SCultSemanticNode> _type;

	public:
		CRAFT_LISP_EXPORTED Variable(instance<SCultSemanticNode> initalizer = instance<>(), instance<SCultSemanticNode> type = instance<>());

		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> initalizerAst() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> typeAst() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) override;

	};

	/******************************************************************************
	** Resolve
	******************************************************************************/

	/*
		Resolves the value of a symbol.
	*/
	class Resolve
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Resolve);
	public:
		enum class Mode
		{
			ResolveOnly = 0,
			ResolveAndGet = 1,
		};

	private:
		instance<Symbol> _symbol;
		Mode _mode;

		instance<Binding> _binding;

	public:
		CRAFT_LISP_EXPORTED Resolve(instance<Symbol> binding, Mode mode = Mode::ResolveOnly);

		CRAFT_LISP_EXPORTED bool isGetter();

		CRAFT_LISP_EXPORTED instance<Symbol> getSymbol() const;
		CRAFT_LISP_EXPORTED instance<Binding> getBinding() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
	};

	/******************************************************************************
	** Block
	******************************************************************************/

	/*
		A do block node.
	*/
	class Block
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Block);
	private:
		instance<SScope> _parentScope;
		// TODO: Symbol equality
		std::vector<instance<Binding>> _bindings;
		std::map<size_t, size_t> _symbolTable; // Internal table

	private:
		std::vector<instance<SCultSemanticNode>> _statements;

	public:
		CRAFT_LISP_EXPORTED Block();

		CRAFT_LISP_EXPORTED void preSize(size_t);
		CRAFT_LISP_EXPORTED void push(instance<SCultSemanticNode>);

		CRAFT_LISP_EXPORTED size_t statementCount() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> statementAst(size_t index) const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;

		// SScope
	public:
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const override;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<BindSite> ast) override;
	};
}}
