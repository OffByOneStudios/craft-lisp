#include "lisp/common.h"
#include "nonce.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;

CRAFT_DEFINE(Nonce)
{
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		size_t bin_len;

		std::string buf;
		buf.resize(s.size());

		auto res = instance<Nonce>::make();
		sodium_base642bin((uint8_t*)buf.c_str(), crypto_secretbox_NONCEBYTES,
			s.c_str(), s.size(), 0, &bin_len, 0, sodium_base64_VARIANT_URLSAFE);

		if (bin_len != crypto_secretbox_NONCEBYTES)
			throw stdext::exception("Invalid String Length for SecretBoxNonce");

		memcpy(res->nonce, buf.data(), bin_len);
		return res;
	});

	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Nonce> l) -> std::string {
		auto mlen = sodium_base64_ENCODED_LEN(crypto_secretbox_NONCEBYTES, sodium_base64_VARIANT_URLSAFE);
		std::string res; res.resize(mlen);
		sodium_bin2base64(const_cast<char*>(res.c_str()), mlen, l->nonce, crypto_secretbox_NONCEBYTES,
			sodium_base64_VARIANT_URLSAFE);
		return res;
	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<Nonce> l) -> std::string
	{
		return l.getFeature<PRepr>()->toRepr(l);
	});
	_.defaults();
}

Nonce::Nonce()
{
	randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);
}