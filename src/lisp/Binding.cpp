#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Binding.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Binding)
{
	_.use<SBinding>().byCasting();

	_.defaults();
}

Binding::Binding(std::string name, instance<> expression)
{
	_name = name;
	_expression = expression;
}

std::string Binding::name() const
{
	return _name;
}

instance<> Binding::expression()
{
	return _expression;
}

void Binding::setValue(instance<> value)
{
	_value = value;
}
instance<> Binding::value()
{
	return _value;
}

instance<> Binding::eval(instance<SFrame> frame)
{
	_value = frame->getNamespace()->environment()->eval(frame, _expression);

	return _value;
}

instance<> Binding::getValue(instance<SFrame> frame) const
{
	return _value;
}

void Binding::addMeta(std::string metaKey, instance<> value)
{
	_meta[metaKey] = value;
}

instance<> Binding::getMeta(std::string metaKey, TypeId type)
{
	auto it = _meta.find(metaKey);

	if (it != _meta.end()
		&& (type == types::None || it->second.typeId() == type))
		return it->second;

	return instance<>();
}
