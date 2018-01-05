#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Binding.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Binding)
{
	_.defaults();
}

Binding::Binding(std::string name, instance<> value)
{
	_name = name;
	_value = value;
}

instance<> Binding::eval(instance<SScope> scope)
{
	_value = scope->environment()->eval(scope, _value);

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
