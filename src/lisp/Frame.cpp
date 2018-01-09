#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Frame.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Frame)
{
	_.use<SFrame>().byCasting();

	_.defaults();
}


Frame::Frame(instance<Execution> exe)
{
	_execution = exe;
	_parent = instance<>();
}

Frame::Frame(instance<SFrame> parent, instance<SScope> scope)
{
	_execution = parent->execution();
	_parent = parent;

	_scope = scope;
}


instance<Environment> Frame::environment() const
{
	return _execution->environment();
}
instance<SFrame> Frame::parent() const
{
	return _parent;
}
instance<SFrame> Frame::lexical_parent() const
{
	return _parent_lexical;
}

instance<SScope> Frame::scope() const
{
	return _scope;
}
instance<Execution> Frame::execution() const
{
	return _execution;
}

size_t Frame::push(instance<> value)
{
	_values.push_back(value);
	return _values.size() - 1;
}
instance<> Frame::get(size_t index)
{
	return _values[index];
}
void Frame::set(size_t index, instance<> value)
{
	if (index >= _values.size())
		_values.resize(index + 1);

	_values[index] = value;
}

void Frame::setLexicalParent(instance<SFrame> value)
{
	_parent_lexical = value;
}