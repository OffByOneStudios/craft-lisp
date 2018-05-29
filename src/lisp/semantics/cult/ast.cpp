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

std::string SCultSemanticNode::sourceLocationToString() const
{
	auto sem = SScope::findScope(_parent)->getSemantics();
	auto source = sem->getSource();
	std::string res;

	if (source && source.isType<CultLispSyntax>())
	{
		auto syntax = source.asType<CultLispSyntax>();
		auto sourceLoc = syntax->getSourceLocation(_source_start, _source_end);

		if (sourceLoc.length == 1)
			res = fmt::format("{0}:{1}", sourceLoc.start_line, sourceLoc.start_col);
		else if (sourceLoc.length > 1)
			res = fmt::format("{0}:{1} {2}", sourceLoc.start_line, sourceLoc.start_col, sourceLoc.length);
	}
	if (res.empty())
		res = "???";

	return fmt::format("{0}/{1}", sem->getModule()->uri(), res);
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
