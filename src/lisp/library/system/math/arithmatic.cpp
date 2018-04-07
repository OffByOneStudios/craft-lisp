
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "../prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void system::make_arithmatic_globals(instance<Module>& ret, instance<Namespace>& ns) {
	auto env = ns->environment();
	auto _operator_div = instance<MultiMethod>::make();
	auto _operator_mul = instance<MultiMethod>::make();
	auto _operator_plu = instance<MultiMethod>::make();
	auto _operator_sub = instance<MultiMethod>::make();

	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<float, float, float>(), [](auto frame, auto args) { instance<float> a(expect<float>(args[0])); instance<float> b(expect<float>(args[1])); return instance<float>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<double, double, double>(), [](auto frame, auto args) { instance<double> a(expect<double>(args[0])); instance<double> b(expect<double>(args[1])); return instance<double>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(), [](auto frame, auto args) { instance<int16_t> a(expect<int16_t>(args[0])); instance<int16_t> b(expect<int16_t>(args[1])); return instance<int16_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(), [](auto frame, auto args) { instance<int32_t> a(expect<int32_t>(args[0])); instance<int32_t> b(expect<int32_t>(args[1])); return instance<int32_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(), [](auto frame, auto args) { instance<int64_t> a(expect<int64_t>(args[0])); instance<int64_t> b(expect<int64_t>(args[1])); return instance<int64_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(), [](auto frame, auto args) { instance<int8_t> a(expect<int8_t>(args[0])); instance<int8_t> b(expect<int8_t>(args[1])); return instance<int8_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(), [](auto frame, auto args) { instance<uint16_t> a(expect<uint16_t>(args[0])); instance<uint16_t> b(expect<uint16_t>(args[1])); return instance<uint16_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(), [](auto frame, auto args) { instance<uint32_t> a(expect<uint32_t>(args[0])); instance<uint32_t> b(expect<uint32_t>(args[1])); return instance<uint32_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(), [](auto frame, auto args) { instance<uint64_t> a(expect<uint64_t>(args[0])); instance<uint64_t> b(expect<uint64_t>(args[1])); return instance<uint64_t>::make(*a / *b); }));
	_operator_div->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(), [](auto frame, auto args) { instance<uint8_t> a(expect<uint8_t>(args[0])); instance<uint8_t> b(expect<uint8_t>(args[1])); return instance<uint8_t>::make(*a / *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<float, float, float>(), [](auto frame, auto args) { instance<float> a(expect<float>(args[0])); instance<float> b(expect<float>(args[1])); return instance<float>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<double, double, double>(), [](auto frame, auto args) { instance<double> a(expect<double>(args[0])); instance<double> b(expect<double>(args[1])); return instance<double>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(), [](auto frame, auto args) { instance<int16_t> a(expect<int16_t>(args[0])); instance<int16_t> b(expect<int16_t>(args[1])); return instance<int16_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(), [](auto frame, auto args) { instance<int32_t> a(expect<int32_t>(args[0])); instance<int32_t> b(expect<int32_t>(args[1])); return instance<int32_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(), [](auto frame, auto args) { instance<int64_t> a(expect<int64_t>(args[0])); instance<int64_t> b(expect<int64_t>(args[1])); return instance<int64_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(), [](auto frame, auto args) { instance<int8_t> a(expect<int8_t>(args[0])); instance<int8_t> b(expect<int8_t>(args[1])); return instance<int8_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(), [](auto frame, auto args) { instance<uint16_t> a(expect<uint16_t>(args[0])); instance<uint16_t> b(expect<uint16_t>(args[1])); return instance<uint16_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(), [](auto frame, auto args) { instance<uint32_t> a(expect<uint32_t>(args[0])); instance<uint32_t> b(expect<uint32_t>(args[1])); return instance<uint32_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(), [](auto frame, auto args) { instance<uint64_t> a(expect<uint64_t>(args[0])); instance<uint64_t> b(expect<uint64_t>(args[1])); return instance<uint64_t>::make(*a * *b); }));
	_operator_mul->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(), [](auto frame, auto args) { instance<uint8_t> a(expect<uint8_t>(args[0])); instance<uint8_t> b(expect<uint8_t>(args[1])); return instance<uint8_t>::make(*a * *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<float, float, float>(), [](auto frame, auto args) { instance<float> a(expect<float>(args[0])); instance<float> b(expect<float>(args[1])); return instance<float>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<double, double, double>(), [](auto frame, auto args) { instance<double> a(expect<double>(args[0])); instance<double> b(expect<double>(args[1])); return instance<double>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(), [](auto frame, auto args) { instance<int16_t> a(expect<int16_t>(args[0])); instance<int16_t> b(expect<int16_t>(args[1])); return instance<int16_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(), [](auto frame, auto args) { instance<int32_t> a(expect<int32_t>(args[0])); instance<int32_t> b(expect<int32_t>(args[1])); return instance<int32_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(), [](auto frame, auto args) { instance<int64_t> a(expect<int64_t>(args[0])); instance<int64_t> b(expect<int64_t>(args[1])); return instance<int64_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(), [](auto frame, auto args) { instance<int8_t> a(expect<int8_t>(args[0])); instance<int8_t> b(expect<int8_t>(args[1])); return instance<int8_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(), [](auto frame, auto args) { instance<uint16_t> a(expect<uint16_t>(args[0])); instance<uint16_t> b(expect<uint16_t>(args[1])); return instance<uint16_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(), [](auto frame, auto args) { instance<uint32_t> a(expect<uint32_t>(args[0])); instance<uint32_t> b(expect<uint32_t>(args[1])); return instance<uint32_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(), [](auto frame, auto args) { instance<uint64_t> a(expect<uint64_t>(args[0])); instance<uint64_t> b(expect<uint64_t>(args[1])); return instance<uint64_t>::make(*a + *b); }));
	_operator_plu->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(), [](auto frame, auto args) { instance<uint8_t> a(expect<uint8_t>(args[0])); instance<uint8_t> b(expect<uint8_t>(args[1])); return instance<uint8_t>::make(*a + *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<float, float, float>(), [](auto frame, auto args) { instance<float> a(expect<float>(args[0])); instance<float> b(expect<float>(args[1])); return instance<float>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<double, double, double>(), [](auto frame, auto args) { instance<double> a(expect<double>(args[0])); instance<double> b(expect<double>(args[1])); return instance<double>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(), [](auto frame, auto args) { instance<int16_t> a(expect<int16_t>(args[0])); instance<int16_t> b(expect<int16_t>(args[1])); return instance<int16_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(), [](auto frame, auto args) { instance<int32_t> a(expect<int32_t>(args[0])); instance<int32_t> b(expect<int32_t>(args[1])); return instance<int32_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(), [](auto frame, auto args) { instance<int64_t> a(expect<int64_t>(args[0])); instance<int64_t> b(expect<int64_t>(args[1])); return instance<int64_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(), [](auto frame, auto args) { instance<int8_t> a(expect<int8_t>(args[0])); instance<int8_t> b(expect<int8_t>(args[1])); return instance<int8_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(), [](auto frame, auto args) { instance<uint16_t> a(expect<uint16_t>(args[0])); instance<uint16_t> b(expect<uint16_t>(args[1])); return instance<uint16_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(), [](auto frame, auto args) { instance<uint32_t> a(expect<uint32_t>(args[0])); instance<uint32_t> b(expect<uint32_t>(args[1])); return instance<uint32_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(), [](auto frame, auto args) { instance<uint64_t> a(expect<uint64_t>(args[0])); instance<uint64_t> b(expect<uint64_t>(args[1])); return instance<uint64_t>::make(*a - *b); }));
	_operator_sub->attach(env, instance<BuiltinFunction>::make(SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(), [](auto frame, auto args) { instance<uint8_t> a(expect<uint8_t>(args[0])); instance<uint8_t> b(expect<uint8_t>(args[1])); return instance<uint8_t>::make(*a - *b); }));

	ret->define_eval("/", _operator_div);
	ret->define_eval("*", _operator_mul);
	ret->define_eval("+", _operator_plu);
	ret->define_eval("-", _operator_sub);
}

