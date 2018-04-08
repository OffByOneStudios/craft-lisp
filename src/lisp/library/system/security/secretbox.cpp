#include "lisp/common.h"
#include "secretbox.h"

#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

CRAFT_OBJECT_DEFINE(SecretBoxKey)
{
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		size_t bin_len;

		std::string buf;
		buf.resize(s.size());
		
		auto res = instance<SecretBoxKey>::make();
		sodium_base642bin((uint8_t*)buf.c_str(), crypto_secretbox_KEYBYTES,
			s.c_str(), s.size(), 0, &bin_len, 0, sodium_base64_VARIANT_URLSAFE);

		if (bin_len != crypto_secretbox_KEYBYTES)
			throw stdext::exception("Invalid String Length for SecretBoxKey");

		memcpy(res->key, buf.data(), bin_len);
		return res;
	});

	_.use<PStringer>().singleton<FunctionalStringer>([](instance<SecretBoxKey> l) -> std::string {
		auto mlen = sodium_base64_ENCODED_LEN(crypto_secretbox_KEYBYTES, sodium_base64_VARIANT_URLSAFE);
		std::string res; res.resize(mlen);
		sodium_bin2base64(const_cast<char*>(res.c_str()), mlen, l->key, crypto_secretbox_KEYBYTES,
			sodium_base64_VARIANT_URLSAFE);
		return res;
	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<SecretBoxKey> l) -> std::string
	{
		return l.getFeature<PRepr>()->toRepr(l);
	});
	_.defaults();
}



CRAFT_OBJECT_DEFINE(SecretBoxCipher)
{
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		auto res = instance<SecretBoxCipher>::make();
		res->cipher.resize(s.size());

		size_t bin_len;
		sodium_base642bin(res->cipher.data(), s.size(),
			s.c_str(), s.size(), 0, &bin_len, 0, sodium_base64_VARIANT_URLSAFE);
		res->cipher.resize(bin_len);
		return res;
	});

	_.use<PStringer>().singleton<FunctionalStringer>([](instance<SecretBoxCipher> l) -> std::string {
		auto mlen = sodium_base64_ENCODED_LEN(l->cipher.size(), sodium_base64_VARIANT_URLSAFE);
		std::string res; res.resize(mlen);
		sodium_bin2base64(const_cast<char*>(res.c_str()), mlen, l->cipher.data(), l->cipher.size(),
			sodium_base64_VARIANT_URLSAFE);
		return res;
	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<SecretBoxCipher> l) -> std::string
	{
		return l.getFeature<PRepr>()->toRepr(l);
	});
	_.defaults();
}


SecretBoxKey::SecretBoxKey()
{
	crypto_secretbox_keygen(key);
}

Nonce::Nonce()
{
	randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);
}

SecretBoxCipher::SecretBoxCipher(instance<SecretBoxKey> key, instance<Nonce> nonce, instance<std::string> message)
{
	cipher.resize(crypto_secretbox_MACBYTES + message->size());
	crypto_secretbox_easy(cipher.data(), (unsigned char*)(message->c_str()), message->size(), nonce->nonce, key->key);
}


void system::make_secretkey_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto boxkeygen = instance<MultiMethod>::make();
	boxkeygen->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<SecretBoxKey>(),
		[](auto frame, auto args)
	{
		return instance<SecretBoxKey>::make();
	}));
	ret->define_eval("salsa/keygen", boxkeygen);

	auto boxencrypt = instance<MultiMethod>::make();
	boxencrypt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, SecretBoxKey, Nonce, SecretBoxCipher>(),
		[](auto frame, auto args)
	{
		auto a = args[0].asType<std::string>();
		auto b = args[1].asType<SecretBoxKey>();
		auto c = args[2].asType<Nonce>();
		
		return instance<SecretBoxCipher>::make(b, c, a);
	}));
	ret->define_eval("salsa/encrypt", boxencrypt);

	auto boxdecrypt = instance<MultiMethod>::make();
	boxdecrypt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<SecretBoxCipher, SecretBoxKey, Nonce, std::string>(),
		[](auto frame, auto args)
	{
		auto cipher = args[0].asType<SecretBoxCipher>();
		auto key = args[1].asType<SecretBoxKey>();
		auto nonce = args[2].asType<Nonce>();

		size_t outsize = cipher->cipher.size() - crypto_secretbox_MACBYTES;
		std::string res;
		res.resize(outsize);
		if (crypto_secretbox_open_easy((uint8_t*)res.c_str(), cipher->cipher.data(), cipher->cipher.size(), nonce->nonce, key->key) != 0) {
			throw stdext::exception("Message Forgary");
		}
		return instance<std::string>::make(res);
	}));
	ret->define_eval("salsa/decrypt", boxdecrypt);


}