#include "lisp/common.h"
#include "lisp/lisp.h"
#include "slots.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

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

size_t RuntimeSlots::getSize(instance<>* inst)
{
	return inst->asType<RuntimeSlots>()->size;
}
instance<>* RuntimeSlots::getSlot(instance<>* inst, size_t index)
{
	assert(index < inst->asType<RuntimeSlots>()->size);
	return &(inst->asType<RuntimeSlots>()->slots[index]);
}

instance<> RuntimeSlots::getLastSlot(instance<>* inst)
{
	auto slots = inst->asType<RuntimeSlots>();

	return slots->slots[slots->size - 1];
}

void RuntimeSlots::extend(instance<>* inst, size_t size)
{
	auto slots = inst->asType<RuntimeSlots>();

	assert(size >= slots->size);

	auto new_slots = new instance<>[size];

	for (auto i = 0; i < slots->size; ++i)
		new_slots[i] == slots->slots[i];

	slots->size = size;
	std::swap(slots->slots, new_slots);

	delete[] new_slots;
}
