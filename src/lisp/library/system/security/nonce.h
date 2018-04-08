#pragma once
#define SODIUM_STATIC
#include "lisp/common.h"
#include "sodium/sodium.h"

namespace craft {
namespace lisp {
namespace library {
	class Nonce
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Nonce);

	public:
		unsigned char nonce[crypto_secretbox_NONCEBYTES];

		CRAFT_LISP_EXPORTED Nonce();
	};
}}}