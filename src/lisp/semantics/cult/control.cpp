#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/control.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** Condition
******************************************************************************/

CRAFT_DEFINE(Condition)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<Condition> that)
	{
		auto clone = instance<Condition>::make();

		auto count = that->branchCount();
		clone->preSize(count);
		for (auto i = 0; i < count; ++i)
			clone->push(_clone(that->branchConditionAst(i)), _clone(that->branchAst(i)));
		auto defaultBranch = that->branchDefaultAst();
		if (defaultBranch) clone->push(instance<>(), defaultBranch);

		return clone;
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}


Condition::Condition()
{

}

void Condition::preSize(size_t cap)
{
	_entries.reserve(cap);
}
void Condition::push(instance<SCultSemanticNode> cond, instance<SCultSemanticNode> branch)
{
	if (!cond)
	{
		if (_defaultBranch)
			throw ast_error("Node `{0}` (Condition) already has a default branch.");
		_defaultBranch = branch;
	}
	else
	{
		_entries.push_back({ _ast(cond) , _ast(branch) });
	}
}

size_t Condition::branchCount() const
{
	return _entries.size();
}
instance<SCultSemanticNode> Condition::branchConditionAst(size_t index) const
{
	return _entries[index].condition;
}
instance<SCultSemanticNode> Condition::branchAst(size_t index) const
{
	return _entries[index].branch;
}
instance<SCultSemanticNode> Condition::branchDefaultAst() const
{
	return _defaultBranch;
}

void Condition::bind()
{
	for (auto entry : _entries)
	{
		entry.branch->bind();
		entry.condition->bind();
	}

	if (_defaultBranch) _defaultBranch->bind();
}

/******************************************************************************
** Loop
******************************************************************************/

CRAFT_DEFINE(Loop)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<Loop> that)
	{
		return instance<Loop>::make(_clone(that->conditionAst()), _clone(that->bodyAst()));
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}


Loop::Loop(instance<SCultSemanticNode> cond, instance<SCultSemanticNode> body)
{
	_condition = cond;
	_body = body;
}

void Loop::craft_setupInstance()
{
	_ast(_condition);
	_ast(_body);
}

instance<SCultSemanticNode> Loop::conditionAst() const
{
	return _condition;
}
instance<SCultSemanticNode> Loop::bodyAst() const
{
	return _body;
}

void Loop::bind()
{
	_condition->bind();
	_body->bind();
}