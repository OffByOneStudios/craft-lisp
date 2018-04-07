#pragma once
#define SODIUM_STATIC
#include "sodium/sodium.h"

namespace craft {
namespace lisp {
namespace library {
	class SecretBoxKey
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::SecretBoxKey);
		
	public:
		unsigned char key[crypto_secretbox_KEYBYTES];
		CRAFT_LISP_EXPORTED SecretBoxKey();
	};

	class SecretBoxNonce
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::SecretBoxNonce);

	public:
		unsigned char nonce[crypto_secretbox_NONCEBYTES];

		CRAFT_LISP_EXPORTED SecretBoxNonce();
	};

	class SecretBoxCipher
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::SecretBoxCipher);

	public:
		std::vector<uint8_t> cipher;

		SecretBoxCipher() = default;
		SecretBoxCipher(instance<SecretBoxKey> key, instance<SecretBoxNonce> nonce, instance<std::string> message);
	};
}}}