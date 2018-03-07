#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/BlockBinding.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(BlockBinding)
{
	_.use<SBinding>().byCasting();

	_.defaults();
}

BlockBinding::BlockBinding(size_t pos, std::string name, instance<> expression)
{
	_pos = pos;
	_name = name;
	_expression = expression;
}

size_t BlockBinding::position() const
{
	return _pos;
}
std::string BlockBinding::name() const
{
	return _name;
}
instance<> BlockBinding::getValue(instance<SFrame> frame) const
{
	instance<Frame> block = frame;

	return block->get(_pos);
}

instance<> BlockBinding::eval(instance<SFrame> frame)
{
	instance<Frame> block = frame;

	auto value = frame->getNamespace()->environment()->eval(frame, _expression);
	block->set(_pos, value);

	return value;
}

void BlockBinding::addMeta(std::string metaKey, instance<> value)
{
	_meta[metaKey] = value;
}

instance<> BlockBinding::getMeta(std::string metaKey, TypeId type)
{
	auto it = _meta.find(metaKey);

	if (it != _meta.end()
		&& (type == types::None || it->second.typeId() == type))
		return it->second;

	return instance<>();
}
