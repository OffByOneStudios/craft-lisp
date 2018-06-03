#include "lisp/common.h"
#include "lisp/lisp.h"
#include "slots.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** RuntimeSlots
******************************************************************************/

CRAFT_DEFINE(RuntimeSlots)
{
	_.defaults();
}

RuntimeSlots::RuntimeSlots(instance<> representing, size_t size)
{
	this->representing = representing;
	this->size = size;
	this->slots = new instance<>[size];
}

RuntimeSlots::~RuntimeSlots()
{
	delete[] this->slots;
}

size_t RuntimeSlots::getSize()
{
	return size;
}
instance<>* RuntimeSlots::getSlot(size_t index)
{
	assert(index < size);
	return slots + index;
}
instance<> RuntimeSlots::getLastSlot()
{
	return slots[size - 1];
}
void RuntimeSlots::extend(size_t resize)
{
	assert(resize >= size);

	auto new_slots = new instance<>[resize];

	for (auto i = 0; i < size; ++i)
		new_slots[i] = slots[i];

	size = resize;
	std::swap(slots, new_slots);

	delete[] new_slots;
}

size_t RuntimeSlots::getSize(instance<>* inst)
{
	return ((RuntimeSlots*)inst->get())->getSize();
}
instance<>* RuntimeSlots::getSlot(instance<>* inst, size_t index)
{
	return ((RuntimeSlots*)inst->get())->getSlot(index);
}

instance<> RuntimeSlots::getLastSlot(instance<>* inst)
{
	return ((RuntimeSlots*)inst->get())->getLastSlot();
}

void RuntimeSlots::extend(instance<>* inst, size_t size)
{
	((RuntimeSlots*)inst->get())->extend(size);
}

/******************************************************************************
** RuntimeSlotReference
******************************************************************************/

CRAFT_DEFINE(RuntimeSlotReference)
{
	_.defaults();
}

RuntimeSlotReference::RuntimeSlotReference(instance<> slots, size_t index)
{
	_slots = slots;
	_index = index;
}

instance<> RuntimeSlotReference::getValue()
{
	return *RuntimeSlots::getSlot(&_slots, _index);
}
void RuntimeSlotReference::setValue(instance<> value)
{
	*RuntimeSlots::getSlot(&_slots, _index) = value;
}