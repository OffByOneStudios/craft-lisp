
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "../prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void core::make_logic_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("<", [](instance<int32_t> a, instance<int32_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<int16_t> a, instance<int16_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<bool> a, instance<bool> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<double> a, instance<double> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<int64_t> a, instance<int64_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<float> a, instance<float> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<", [](instance<int8_t> a, instance<int8_t> b) -> instance<bool> { return instance<bool>::make(*a < *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<int32_t> a, instance<int32_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<int16_t> a, instance<int16_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<bool> a, instance<bool> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<double> a, instance<double> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<int64_t> a, instance<int64_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<float> a, instance<float> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("<=", [](instance<int8_t> a, instance<int8_t> b) -> instance<bool> { return instance<bool>::make(*a <= *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<int32_t> a, instance<int32_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<int16_t> a, instance<int16_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<bool> a, instance<bool> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<double> a, instance<double> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<int64_t> a, instance<int64_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<float> a, instance<float> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("==", [](instance<int8_t> a, instance<int8_t> b) -> instance<bool> { return instance<bool>::make(*a == *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<int32_t> a, instance<int32_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<int16_t> a, instance<int16_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<bool> a, instance<bool> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<double> a, instance<double> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<int64_t> a, instance<int64_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<float> a, instance<float> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod("!=", [](instance<int8_t> a, instance<int8_t> b) -> instance<bool> { return instance<bool>::make(*a != *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<int32_t> a, instance<int32_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<int16_t> a, instance<int16_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<bool> a, instance<bool> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<double> a, instance<double> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<int64_t> a, instance<int64_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<float> a, instance<float> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">=", [](instance<int8_t> a, instance<int8_t> b) -> instance<bool> { return instance<bool>::make(*a >= *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<int32_t> a, instance<int32_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<uint32_t> a, instance<uint32_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<mpz_class> a, instance<mpz_class> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<int16_t> a, instance<int16_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<uint64_t> a, instance<uint64_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<uint8_t> a, instance<uint8_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<bool> a, instance<bool> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<double> a, instance<double> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<uint16_t> a, instance<uint16_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<mpq_class> a, instance<mpq_class> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<int64_t> a, instance<int64_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<float> a, instance<float> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<mpf_class> a, instance<mpf_class> b) -> instance<bool> { return instance<bool>::make(*a > *b); });
	semantics->builtin_implementMultiMethod(">", [](instance<int8_t> a, instance<int8_t> b) -> instance<bool> { return instance<bool>::make(*a > *b); });

}