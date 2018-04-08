#pragma once
#include "lisp/common.h"

#include "lisp/lisp.h"
#define SODIUM_STATIC
#include "sodium/sodium.h"

#include "nonce.h"

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

	class SecretBoxCipher
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::SecretBoxCipher);

	public:
		std::vector<uint8_t> cipher;

		SecretBoxCipher() = default;
		SecretBoxCipher(instance<SecretBoxKey> key, instance<Nonce> nonce, instance<std::string> message);
	};

	namespace system {
		void make_secretkey_globals(instance<Module>& ret, instance<Namespace>& ns);
	}
}}}