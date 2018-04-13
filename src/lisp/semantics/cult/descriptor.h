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
		instance<SCultSemanticNode> _parent;

		instance<Binding> _binding;

	public:

		instance<SCultSemanticNode> _initalizer;
		instance<SCultSemanticNode> _type_ast;

	public:
		CRAFT_LISP_EXPORTED Variable();

		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> initalizerAst() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> typeAst() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;

		// SBindable
	public:
		CRAFT_LISP_EXPORTED virtual instance<Binding> getBinding() const override;
		CRAFT_LISP_EXPORTED virtual void setBinding(instance<Binding>) override;

	};

	/******************************************************************************
	** GetValue
	******************************************************************************/

	/*
	Variable define node
	*/
	class GetValue
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::GetValue);
	private:
		instance<SCultSemanticNode> _parent;

	private:
		instance<Binding> _binding;

	public:
		CRAFT_LISP_EXPORTED GetValue(instance<Binding> binding);

		CRAFT_LISP_EXPORTED instance<Binding> getBinding() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;
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
		instance<SCultSemanticNode> _parent;

		instance<SScope> _parentSymbols;
		// TODO: Symbol equality
		std::map<size_t, instance<Binding>> _symbolTable; // Internal table

	private:
		std::vector<instance<SCultSemanticNode>> _statements;

	public:
		CRAFT_LISP_EXPORTED Block(instance<SScope> parent_scope);

		CRAFT_LISP_EXPORTED void preSize(size_t);
		CRAFT_LISP_EXPORTED void push(instance<SCultSemanticNode>);

		CRAFT_LISP_EXPORTED size_t statementCount() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> statementAst(size_t index) const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;

		// SScope
	public:
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const override;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<BindSite> ast) override;
	};
}}
