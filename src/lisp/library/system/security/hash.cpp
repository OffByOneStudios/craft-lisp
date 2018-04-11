#include "lisp/common.h"
#include "hash.h"

#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

#include "PHashable.hpp"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

CRAFT_DEFINE(Hash)
{
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		size_t bin_len;

		std::string buf;
		buf.resize(s.size());
		
		auto res = instance<Hash>::make();
		sodium_base642bin((uint8_t*)buf.c_str(), crypto_generichash_BYTES,
			s.c_str(), s.size(), 0, &bin_len, 0, sodium_base64_VARIANT_URLSAFE);

		if (bin_len != crypto_generichash_BYTES)
			throw stdext::exception("Invalid String Length for Hash");

		memcpy(res->hash, buf.data(), bin_len);
		return res;
	});

	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Hash> l) -> std::string {
		auto mlen = sodium_base64_ENCODED_LEN(crypto_generichash_BYTES, sodium_base64_VARIANT_URLSAFE);
		std::string res; res.resize(mlen);
		sodium_bin2base64(const_cast<char*>(res.c_str()), mlen, l->hash, crypto_generichash_BYTES,
			sodium_base64_VARIANT_URLSAFE);
		return res;
	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<Hash> l) -> std::string
	{
		return l.getFeature<PRepr>()->toRepr(l);
	});
	_.defaults();
}

CRAFT_DEFINE(PHashable) { _.defaults(); };

#define hsh(type)  (crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(type), NULL, 0))

void core::make_hash_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	auto hash = instance<MultiMethod>::make();

	semantics->builtin_implementMultiMethod("hash", [](instance<mpf_class> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(mpf_class); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<int32_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(int32_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<uint8_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(uint8_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<int8_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(int8_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<uint64_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(uint64_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<uint16_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(uint16_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<float> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(float); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<uint32_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(uint32_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<bool> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(bool); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<double> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(double); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<int64_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(int64_t); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<mpq_class> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(mpq_class); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<mpz_class> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(mpz_class); return res; });
	semantics->builtin_implementMultiMethod("hash", [](instance<int16_t> a) -> instance<Hash> { auto res = instance<Hash>::make(); hsh(int16_t); return res; });
	

	semantics->builtin_implementMultiMethod("hash",
		[](instance<PHashable> a) -> instance<Hash>
	{
		return instance<Hash>::make(a.asFeature<PHashable>()->asHash(a));
	});

}