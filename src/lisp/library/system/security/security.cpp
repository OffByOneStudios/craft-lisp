
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

#include "secretbox.h"



using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_security_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	assert(0 == sodium_init());
	auto env = ns->environment();

	auto hash = instance<MultiMethod>::make();
	hash->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string>(),
		[](auto frame, auto args)
	{

		instance<std::string> a(expect<std::string>(args[0]));
		uint8_t hash[crypto_generichash_BYTES];

		crypto_generichash(hash, sizeof(hash), (const uint8_t*)a->c_str(), a->size(), NULL, 0);

		std::string res((const char*)hash, sizeof(hash));
		return instance<std::string>::make(res);

	}));
	ret->define_eval("hash", hash);



	auto boxkeygen = instance<MultiMethod>::make();
	boxkeygen->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<SecretBoxKey>(),
		[](auto frame, auto args)
	{
		return instance<SecretBoxKey>::make();
	}));
	ret->define_eval("sboxkey", boxkeygen);

	auto boxnonce = instance<MultiMethod>::make();
	boxnonce->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<SecretBoxNonce>(),
		[](auto frame, auto args)
	{
		return instance<SecretBoxNonce>::make();
	}));
	ret->define_eval("sboxnonce", boxnonce);

	auto boxencrypt = instance<MultiMethod>::make();
	boxencrypt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<SecretBoxKey, SecretBoxNonce, std::string, SecretBoxCipher>(),
		[](auto frame, auto args)
	{
		auto a = args[0].asType<SecretBoxKey>();
		auto b = args[1].asType<SecretBoxNonce>();
		auto c = args[2].asType<std::string>();
		return instance<SecretBoxCipher>::make(a, b, c);
	}));
	ret->define_eval("sboxencrypt", boxencrypt);

	auto boxdecrypt = instance<MultiMethod>::make();
	boxdecrypt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<SecretBoxKey, SecretBoxNonce, SecretBoxCipher, std::string>(),
		[](auto frame, auto args)
	{
		instance<SecretBoxKey> a = args[0].asType<SecretBoxKey>();
		instance<SecretBoxNonce> b = args[1].asType<SecretBoxNonce>();
		instance<SecretBoxCipher> c = args[2].asType<SecretBoxCipher>();

		size_t outsize = c->cipher.size() - crypto_secretbox_MACBYTES;
		std::string res;
		res.resize(outsize);
		if (crypto_secretbox_open_easy((uint8_t*)res.c_str(), c->cipher.data(), c->cipher.size(), b->nonce, a->key) != 0) {
			throw stdext::exception("Message Forgary");
		}
		return instance<std::string>::make(res);
	}));
	ret->define_eval("sboxdecrypt", boxdecrypt);
}
