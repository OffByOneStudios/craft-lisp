#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/*
		A basic slots object for representing instances dynamically
	*/
	class BasicSlots
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BasicSlots);
	private:
		size_t size;
		instance<>* slots;

		instance<> representing;

	public:
		CRAFT_LISP_EXPORTED BasicSlots(instance<> representing, size_t size);
		CRAFT_LISP_EXPORTED ~BasicSlots();

		static CRAFT_LISP_EXPORTED size_t getSize(instance<>*);
		static CRAFT_LISP_EXPORTED instance<>* getSlots(instance<>*);
	};

}}
