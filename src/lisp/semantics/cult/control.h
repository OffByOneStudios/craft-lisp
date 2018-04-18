#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** Condition
	******************************************************************************/

	/*
		Condition branch node
	*/
	class Condition
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Condition);
	private:
		instance<SCultSemanticNode> _parent;

	public:
		struct _Entry
		{
			instance<SCultSemanticNode> condition;
			instance<SCultSemanticNode> branch;
		};

		std::vector<_Entry> _entries;
		instance<SCultSemanticNode> _defaultBranch;

	public:
		CRAFT_LISP_EXPORTED Condition();

		CRAFT_LISP_EXPORTED void preSize(size_t);
		CRAFT_LISP_EXPORTED void push(instance<SCultSemanticNode> cond, instance<SCultSemanticNode> branch);

		CRAFT_LISP_EXPORTED size_t branchCount() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> branchConditionAst(size_t index) const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> branchAst(size_t index) const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> branchDefaultAst() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
	};

	/******************************************************************************
	** Loop
	******************************************************************************/

	/*
		While loop branch node
	*/
	class Loop
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Loop);

	private:

		instance<SCultSemanticNode> _condition;
		instance<SCultSemanticNode> _body;

	public:
		CRAFT_LISP_EXPORTED Loop(instance<SCultSemanticNode> cond, instance<SCultSemanticNode> body);

		CRAFT_LISP_EXPORTED void craft_setupInstance();

		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> conditionAst() const;
		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> bodyAst() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual void bind() override;
	};
}}
