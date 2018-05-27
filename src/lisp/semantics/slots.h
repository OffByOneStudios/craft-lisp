#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/*
		A basic slots object for representing dynamic runtime objects.

		Used for:
		* Modules (Regardless of backend)
		* Interpreter Closures
		* Interpreter Frames
	*/
	class RuntimeSlots
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::RuntimeSlots);
	private:
		size_t size;
		instance<>* slots;

		instance<> representing;

	public:
		CRAFT_LISP_EXPORTED RuntimeSlots(instance<> representing, size_t size);
		CRAFT_LISP_EXPORTED ~RuntimeSlots();

		CRAFT_LISP_EXPORTED size_t getSize();
		CRAFT_LISP_EXPORTED instance<>* getSlot(size_t index);
		CRAFT_LISP_EXPORTED instance<> getLastSlot();
		CRAFT_LISP_EXPORTED void extend(size_t size);

		static CRAFT_LISP_EXPORTED size_t getSize(instance<>*);
		static CRAFT_LISP_EXPORTED instance<>* getSlot(instance<>*, size_t index);
		static CRAFT_LISP_EXPORTED instance<> getLastSlot(instance<>*);
		static CRAFT_LISP_EXPORTED void extend(instance<>*, size_t size);
	};

	/* A temporary slot helper
	*/
	class RuntimeSlotReference
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::RuntimeSlotReference);
	private:
		instance<RuntimeSlots> _slots;
		size_t _index;

	public:
		CRAFT_LISP_EXPORTED RuntimeSlotReference(instance<RuntimeSlots> slots, size_t index);

		CRAFT_LISP_EXPORTED instance<> getValue();
		CRAFT_LISP_EXPORTED void setValue(instance<> value);
	};
}}
