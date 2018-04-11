#pragma once
#include "lisp/common.h"

#include "lisp/lisp.h"
#define SODIUM_STATIC
#include "sodium/sodium.h"

#include "nonce.h"

namespace craft {
namespace lisp {
namespace library {
	class PrivateKey
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::PrivateKey);
		
	public:
		unsigned char key[crypto_box_SECRETKEYBYTES];
	};

	class PublicKey
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::PublicKey);

	public:
		unsigned char key[crypto_box_PUBLICKEYBYTES];
	};
	
	class Keypair
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Keypair);

	public:
		Keypair();

		instance<PrivateKey> private_;
		instance<PublicKey> public_;
	};

	class KeypairCipher
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::KeypairCipher);

	public:
		std::vector<uint8_t> cipher;

		KeypairCipher() = default;
		KeypairCipher(instance<PublicKey> pub, instance<PrivateKey> priv, instance<Nonce> nonce, instance<std::string> message);
	};

	namespace core {
		void make_publickey_globals(instance<Module> ret);
	}
}}}