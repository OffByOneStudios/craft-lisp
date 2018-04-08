
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

#include "secretbox.h"
#include "hash.h"
#include "publickey.h"


using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_security_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	assert(0 == sodium_init());
	auto env = ns->environment();

	system::make_hash_globals(ret, ns);
	system::make_secretkey_globals(ret, ns);
	system::make_publickey_globals(ret, ns);

	auto nonce = instance<MultiMethod>::make();
	nonce->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<Nonce>(),
		[](auto frame, auto args)
	{
		return instance<Nonce>::make();
	}));
	ret->define_eval("security/nonce", nonce);
}
