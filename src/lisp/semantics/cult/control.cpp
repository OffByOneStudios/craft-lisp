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
		_entries.push_back({ _ast(craft_instance(), cond) , _ast(craft_instance(), branch) });
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

instance<SCultSemanticNode> Condition::getParent() const
{
	return _parent;
}
void Condition::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

/******************************************************************************
** Loop
******************************************************************************/

CRAFT_DEFINE(Loop)
{
	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}


Loop::Loop(instance<SCultSemanticNode> cond, instance<SCultSemanticNode> body)
{
	_condition = _ast(craft_instance(), cond);
	_body = _ast(craft_instance(), body);
}

instance<SCultSemanticNode> Loop::conditionAst() const
{
	return _condition;
}
instance<SCultSemanticNode> Loop::bodyAst() const
{
	return _body;
}

instance<SCultSemanticNode> Loop::getParent() const
{
	return _parent;
}
void Loop::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}
