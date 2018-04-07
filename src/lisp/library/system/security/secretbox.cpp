#include "lisp/common.h"
#include "secretbox.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;

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

CRAFT_OBJECT_DEFINE(SecretBoxNonce)
{
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		size_t bin_len;

		std::string buf;
		buf.resize(s.size());

		auto res = instance<SecretBoxNonce>::make();
		sodium_base642bin((uint8_t*)buf.c_str(), crypto_secretbox_NONCEBYTES,
			s.c_str(), s.size(), 0, &bin_len, 0, sodium_base64_VARIANT_URLSAFE);

		if (bin_len != crypto_secretbox_NONCEBYTES)
			throw stdext::exception("Invalid String Length for SecretBoxNonce");

		memcpy(res->nonce, buf.data(), bin_len);
		return res;
	});

	_.use<PStringer>().singleton<FunctionalStringer>([](instance<SecretBoxNonce> l) -> std::string {
		auto mlen = sodium_base64_ENCODED_LEN(crypto_secretbox_NONCEBYTES, sodium_base64_VARIANT_URLSAFE);
		std::string res; res.resize(mlen);
		sodium_bin2base64(const_cast<char*>(res.c_str()), mlen, l->nonce, crypto_secretbox_NONCEBYTES,
			sodium_base64_VARIANT_URLSAFE);
		return res;
	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<SecretBoxNonce> l) -> std::string
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

SecretBoxNonce::SecretBoxNonce()
{
	randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);
}

SecretBoxCipher::SecretBoxCipher(instance<SecretBoxKey> key, instance<SecretBoxNonce> nonce, instance<std::string> message)
{
	cipher.resize(crypto_secretbox_MACBYTES + message->size());
	crypto_secretbox_easy(cipher.data(), (unsigned char*)(message->c_str()), message->size(), nonce->nonce, key->key);
}