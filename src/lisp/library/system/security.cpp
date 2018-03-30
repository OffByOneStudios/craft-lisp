
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

#define SODIUM_STATIC
#include "sodium/sodium.h"

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

}
