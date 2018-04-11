
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "../prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void core::make_arithmatic_globals(instance<Module> ret) {
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("+", [](instance<int8_t> a, instance<int8_t> b) -> instance<int8_t> { return instance<int8_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<int16_t> a, instance<int16_t> b) -> instance<int16_t> { return instance<int16_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<int32_t> a, instance<int32_t> b) -> instance<int32_t> { return instance<int32_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<int64_t> a, instance<int64_t> b) -> instance<int64_t> { return instance<int64_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<uint8_t> { return instance<uint8_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<uint16_t> { return instance<uint16_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<uint32_t> { return instance<uint32_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<uint64_t> { return instance<uint64_t>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<float> a, instance<float> b) -> instance<float> { return instance<float>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<double> a, instance<double> b) -> instance<double> { return instance<double>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<mpz_class> { return instance<mpz_class>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<mpq_class> { return instance<mpq_class>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("+", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<mpf_class> { return instance<mpf_class>::make(*a + *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<int8_t> a, instance<int8_t> b) -> instance<int8_t> { return instance<int8_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<int16_t> a, instance<int16_t> b) -> instance<int16_t> { return instance<int16_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<int32_t> a, instance<int32_t> b) -> instance<int32_t> { return instance<int32_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<int64_t> a, instance<int64_t> b) -> instance<int64_t> { return instance<int64_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<uint8_t> { return instance<uint8_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<uint16_t> { return instance<uint16_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<uint32_t> { return instance<uint32_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<uint64_t> { return instance<uint64_t>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<float> a, instance<float> b) -> instance<float> { return instance<float>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<double> a, instance<double> b) -> instance<double> { return instance<double>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<mpz_class> { return instance<mpz_class>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<mpq_class> { return instance<mpq_class>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("/", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<mpf_class> { return instance<mpf_class>::make(*a / *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<int8_t> a, instance<int8_t> b) -> instance<int8_t> { return instance<int8_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<int16_t> a, instance<int16_t> b) -> instance<int16_t> { return instance<int16_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<int32_t> a, instance<int32_t> b) -> instance<int32_t> { return instance<int32_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<int64_t> a, instance<int64_t> b) -> instance<int64_t> { return instance<int64_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<uint8_t> { return instance<uint8_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<uint16_t> { return instance<uint16_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<uint32_t> { return instance<uint32_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<uint64_t> { return instance<uint64_t>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<float> a, instance<float> b) -> instance<float> { return instance<float>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<double> a, instance<double> b) -> instance<double> { return instance<double>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<mpz_class> { return instance<mpz_class>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<mpq_class> { return instance<mpq_class>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("-", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<mpf_class> { return instance<mpf_class>::make(*a - *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<int8_t> a, instance<int8_t> b) -> instance<int8_t> { return instance<int8_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<int16_t> a, instance<int16_t> b) -> instance<int16_t> { return instance<int16_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<int32_t> a, instance<int32_t> b) -> instance<int32_t> { return instance<int32_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<int64_t> a, instance<int64_t> b) -> instance<int64_t> { return instance<int64_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<uint8_t> { return instance<uint8_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<uint16_t> { return instance<uint16_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<uint32_t> { return instance<uint32_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<uint64_t> { return instance<uint64_t>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<float> a, instance<float> b) -> instance<float> { return instance<float>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<double> a, instance<double> b) -> instance<double> { return instance<double>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<mpz_class> { return instance<mpz_class>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<mpq_class> { return instance<mpq_class>::make(*a * *b); });
	semantics->builtin_implementMultiMethod("*", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<mpf_class> { return instance<mpf_class>::make(*a * *b); });


}

