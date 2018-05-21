
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "../prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void core::make_cast_globals(instance<Module> ret) {
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("float32", [](instance<uint32_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<uint32_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<uint32_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<uint32_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<uint32_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<uint32_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<uint32_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<uint32_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<uint32_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<float> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<float> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<float> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<float> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<float> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<float> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<float> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<float> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<float> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<int64_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<int64_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<int64_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<int64_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<int64_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<int64_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<int64_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<int64_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<int64_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<uint8_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<uint8_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<uint8_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<uint8_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<uint8_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<uint8_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<uint8_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<uint8_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<uint8_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<double> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<double> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<double> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<double> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<double> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<double> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<double> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<double> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<double> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<uint16_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<uint16_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<uint16_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<uint16_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<uint16_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<uint16_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<uint16_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<uint16_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<uint16_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<int32_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<int32_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<int32_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<int32_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<int32_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<int32_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<int32_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<int32_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<int32_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<int8_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<int8_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<int8_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<int8_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<int8_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<int8_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<int8_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<int8_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<int8_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<int16_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<int16_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<int16_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<int16_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<int16_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<int16_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<int16_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<int16_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("uint64", [](instance<int16_t> a) -> instance<uint64_t> { return instance<uint64_t>::make(uint64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint32", [](instance<uint64_t> a) -> instance<uint32_t> { return instance<uint32_t>::make(uint32_t(*a)); });
	semantics->builtin_implementMultiMethod("float32", [](instance<uint64_t> a) -> instance<float> { return instance<float>::make(float(*a)); });
	semantics->builtin_implementMultiMethod("int64", [](instance<uint64_t> a) -> instance<int64_t> { return instance<int64_t>::make(int64_t(*a)); });
	semantics->builtin_implementMultiMethod("uint8", [](instance<uint64_t> a) -> instance<uint8_t> { return instance<uint8_t>::make(uint8_t(*a)); });
	semantics->builtin_implementMultiMethod("float64", [](instance<uint64_t> a) -> instance<double> { return instance<double>::make(double(*a)); });
	semantics->builtin_implementMultiMethod("uint16", [](instance<uint64_t> a) -> instance<uint16_t> { return instance<uint16_t>::make(uint16_t(*a)); });
	semantics->builtin_implementMultiMethod("int32", [](instance<uint64_t> a) -> instance<int32_t> { return instance<int32_t>::make(int32_t(*a)); });
	semantics->builtin_implementMultiMethod("int8", [](instance<uint64_t> a) -> instance<int8_t> { return instance<int8_t>::make(int8_t(*a)); });
	semantics->builtin_implementMultiMethod("int16", [](instance<uint64_t> a) -> instance<int16_t> { return instance<int16_t>::make(int16_t(*a)); });

}