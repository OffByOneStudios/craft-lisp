#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/semantics/cult/macro.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** Macro
******************************************************************************/

CRAFT_DEFINE(Macro)
{
	_.use<SCultSemanticNode>().byCasting();
	_.use<SBindable>().byCasting();

	_.defaults();
}

Macro::Macro(instance<> macro_subroutine)
{
	_macroSubroutine = macro_subroutine;
}

instance<SCultSemanticNode> Macro::getParent() const
{
	return _parent;
}
void Macro::setParent(instance<SCultSemanticNode> parent)
{
	if (_parent) throw parent_already_set_error(craft_instance());
	_parent = parent;
}

instance<Binding> Macro::getBinding() const
{
	return _binding;
}
void Macro::setBinding(instance<Binding> binding)
{
	_binding = binding;
}
