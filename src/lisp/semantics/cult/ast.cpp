#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/ast.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(craft::lisp::SCultSemanticNode) { _.defaults(); }

instance<SCultSemanticNode> SCultSemanticNode::getParent() const
{
	return _parent;
}
void SCultSemanticNode::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_featuredInstance());
	_parent = parent;
}

void SCultSemanticNode::bind()
{

}

void SCultSemanticNode::validate(ValidationState* vs) const
{
	vs->warning(const_cast<SCultSemanticNode*>(this)->craft_featuredInstance().asFeature<SCultSemanticNode>(), "validate-not-implemented");
}

/******************************************************************************
** Constant
******************************************************************************/

CRAFT_DEFINE(Constant)
{
	_.use<PClone>().singleton<FunctionalCopyConstructor>([](instance<Constant> that)
	{
		return instance<Constant>::make(that->getValue());
	});

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

Constant::Constant(instance<> value)
{
	_value = value;
}

instance<> Constant::getValue() const
{
	return _value;
}
