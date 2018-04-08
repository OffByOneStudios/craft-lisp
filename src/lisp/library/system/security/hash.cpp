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

CRAFT_OBJECT_DEFINE(Hash)
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

CRAFT_ASPECT_DEFINE(PHashable);

void system::make_hash_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto hash = instance<MultiMethod>::make();

	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int8_t, Hash>(), [](auto frame, auto args) {
		instance<int8_t> a(expect<int8_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(int8_t), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int16_t, Hash>(), [](auto frame, auto args) {
		instance<int16_t> a(expect<int16_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(int16_t), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int32_t, Hash>(), [](auto frame, auto args) {
		instance<int32_t> a(expect<int32_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(int32_t), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int64_t, Hash>(), [](auto frame, auto args) {
		instance<int64_t> a(expect<int64_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(int64_t), NULL, 0);

		return res;
	}));


	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint8_t, Hash>(), [](auto frame, auto args) {
		instance<uint8_t> a(expect<uint8_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(uint8_t), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint16_t, Hash>(), [](auto frame, auto args) {
		instance<uint16_t> a(expect<uint16_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(uint16_t), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint32_t, Hash>(), [](auto frame, auto args) {
		instance<uint32_t> a(expect<uint32_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(uint32_t), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint64_t, Hash>(), [](auto frame, auto args) {
		instance<uint64_t> a(expect<uint64_t>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(uint64_t), NULL, 0);

		return res;
	}));


	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<float, Hash>(), [](auto frame, auto args) {
		instance<float> a(expect<float>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(float), NULL, 0);

		return res;
	}));
	hash->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<double, Hash>(), [](auto frame, auto args) {
		instance<double> a(expect<double>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (uint8_t*)a.get(), sizeof(double), NULL, 0);

		return res;
	}));

	hash->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, Hash>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		auto res = instance<Hash>::make();
		crypto_generichash(res->hash, crypto_generichash_BYTES, (const uint8_t*)a->c_str(), a->size(), NULL, 0);

		return res;
	}));

	hash->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<PHashable, Hash>(),
		[](auto frame, std::vector<instance<>> args)
	{
		return instance<Hash>::make(args[0].asFeature<PHashable>()->asHash(args[0]));
	}));

	ret->define_eval("hash", hash);
}