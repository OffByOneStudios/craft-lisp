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
	class BasicSlots
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BasicSlots);
	private:
		size_t size;
		plf::colony<instance<>> slots;

		instance<> representing;

	public:
		CRAFT_LISP_EXPORTED BasicSlots(instance<> representing, size_t size);

		static CRAFT_LISP_EXPORTED size_t getSize(instance<>*);
		static CRAFT_LISP_EXPORTED instance<>* getSlot(instance<>*, size_t index);
	};

}}
