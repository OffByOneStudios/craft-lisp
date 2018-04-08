#pragma once
#include "lisp/common.h"

#include "lisp/lisp.h"
#define SODIUM_STATIC
#include "sodium/sodium.h"

namespace craft {
namespace lisp {
namespace library {
	class Hash
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Hash);
		
	public:
		uint8_t hash[crypto_generichash_BYTES];
	};

	namespace system {
		void make_hash_globals(instance<Module>& ret, instance<Namespace>& ns);
	}
}}}