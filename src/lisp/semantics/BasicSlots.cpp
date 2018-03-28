#include "lisp/common.h"
#include "lisp/lisp.h"
#include "BasicSlots.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(BasicSlots)
{
	_.defaults();
}

BasicSlots::BasicSlots(instance<> representing, size_t size)
{
	this->representing = representing;
	this->size = size;
	this->slots = new instance<>[size];
}
BasicSlots::~BasicSlots()
{
	delete[] this->slots;
}

size_t BasicSlots::getSize(instance<>* inst)
{
	return inst->asType<BasicSlots>()->size;
}
instance<>* BasicSlots::getSlots(instance<>* inst)
{
	return inst->asType<BasicSlots>()->slots;
}