
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


void core::make_security_globals(instance<Module> ret)
{
	assert(0 == sodium_init());
	auto semantics = ret->require<CultSemantics>();


	core::make_hash_globals(ret);
	core::make_secretkey_globals(ret);
	core::make_publickey_globals(ret);

	semantics->builtin_implementMultiMethod("crypto/nonce",
		[]() -> instance<Nonce>
	{
		return instance<Nonce>::make();
	});
}
