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

Frame::Frame()
{
}

Frame::Frame(instance<SScope> scope)
{
	_scope = scope;
}

void Frame::setRepresentative(instance<> value)
{
	_representative = value;
}
void Frame::setLexicalParent(instance<SFrame> value)
{
	_parent_lexical = value;
}

instance<Namespace> Frame::getNamespace() const
{
	return _execution->getNamespace();
}
instance<SFrame> Frame::getPrevious() const
{
	return _execution->stack()[_stack_location - 1];
}
instance<SFrame> Frame::getLexicalParent() const
{
	return _parent_lexical;
}
instance<> Frame::getRepresentative() const
{
	return _representative;
}

instance<SScope> Frame::getScope() const
{
	return _scope;
}
instance<Execution> Frame::getExecution() const
{
	return _execution;
}

void Frame::beginExecution(instance<Execution> value)
{
	_stack_location = value->stack().size() - 1;
	_execution = value;
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