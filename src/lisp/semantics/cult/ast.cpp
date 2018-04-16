#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/ast.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(craft::lisp::SCultSemanticNode) { _.defaults(); }

void SCultSemanticNode::validate(ValidationState* vs) const
{
	vs->warning(const_cast<SCultSemanticNode*>(this)->craft_featuredInstance().asFeature<SCultSemanticNode>(), "Not-Implemented");
}

/******************************************************************************
** Constant
******************************************************************************/

CRAFT_DEFINE(Constant)
{
	_.use<PClone>().singleton<DefaultCopyConstructor>();

	_.use<SCultSemanticNode>().byCasting();

	_.defaults();
}

Constant::Constant(instance<> value)
{
	_value = value;
}
Constant::Constant(Constant const& that)
{
	_value = that._value;
}

instance<> Constant::getValue() const
{
	return _value;
}

instance<SCultSemanticNode> Constant::getParent() const
{
	return _parent;
}
void Constant::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}
