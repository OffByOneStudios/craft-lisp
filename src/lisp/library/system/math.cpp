
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;



void system::make_math_globals(instance<Module>& ret, instance<Namespace>& ns)

{
  auto env = ns->environment();
  //MultiMethods Declarations
    auto _gt = instance<MultiMethod>::make();
    auto _gte = instance<MultiMethod>::make();
    auto _lt = instance<MultiMethod>::make();
    auto _lte = instance<MultiMethod>::make();
    auto _eq = instance<MultiMethod>::make();
    auto _neq = instance<MultiMethod>::make();
    auto _mul = instance<MultiMethod>::make();
    auto _add = instance<MultiMethod>::make();
    auto _div = instance<MultiMethod>::make();
    auto _sub = instance<MultiMethod>::make();
    auto _not = instance<MultiMethod>::make();
    auto _uint64 = instance<MultiMethod>::make();
    auto _int32 = instance<MultiMethod>::make();
    auto _int16 = instance<MultiMethod>::make();
    auto _int64 = instance<MultiMethod>::make();
    auto _uint16 = instance<MultiMethod>::make();
    auto _uint32 = instance<MultiMethod>::make();
    auto _int8 = instance<MultiMethod>::make();
    auto _uint8 = instance<MultiMethod>::make();

  //Binary Operators

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a * *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a + *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a / *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a - *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a * *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a + *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a / *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a - *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, float>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a * *b);
    }));

    _mul->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a * *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, float>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a + *b);
    }));

    _add->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a + *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, float>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a / *b);
    }));

    _div->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a / *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, float>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a - *b);
    }));

    _sub->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a - *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int8_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int16_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int16_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<int64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int8_t> b(expect<int8_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int16_t> b(expect<int16_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int32_t> b(expect<int32_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<int64_t> b(expect<int64_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint8_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint16_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint16_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint32_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<uint64_t>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint8_t> b(expect<uint8_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint16_t> b(expect<uint16_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint32_t> b(expect<uint32_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<uint64_t> b(expect<uint64_t>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a > *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _gte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a >= *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lt->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a < *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _lte->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a <= *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _eq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a == *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<float>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<double>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<double>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<float> b(expect<float>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<double> b(expect<double>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _neq->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	instance<bool> b(expect<bool>(args[1]));
    	return instance<bool>::make(*a != *b);
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, bool>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, bool>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, bool>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

    _not->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<bool, bool>(),
    	[](auto frame, auto args)
    {
    	instance<bool> a(expect<bool>(args[0]));
    	return instance<bool>::make(!(*a));
    }));

  // Unary Operators

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int8_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int8_t> a(expect<int8_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int16_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int16_t> a(expect<int16_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int32_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int32_t> a(expect<int32_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<int64_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<int64_t> a(expect<int64_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint8_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint8_t> a(expect<uint8_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint16_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint16_t> a(expect<uint16_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint32_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint32_t> a(expect<uint32_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<uint64_t, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<uint64_t> a(expect<uint64_t>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _uint64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, uint64_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<uint64_t>::make(uint64_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, int32_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<int32_t>::make(int32_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, int16_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<int16_t>::make(int16_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _int64->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, int64_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<int64_t>::make(int64_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint16->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, uint16_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<uint16_t>::make(uint16_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _uint32->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, uint32_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<uint32_t>::make(uint32_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _int8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, int8_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<int8_t>::make(int8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<float, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<float> a(expect<float>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

    _uint8->attach(env, instance<BuiltinFunction>::make(
    	SubroutineSignature::makeFromArgsAndReturn<double, uint8_t>(),
    	[](auto frame, auto args)
    {
    	instance<double> a(expect<double>(args[0]));
    	return instance<uint8_t>::make(uint8_t(*a));
    }));

  // Mounts
    ret->define_eval(">", _gt);
    ret->define_eval(">=", _gte);
    ret->define_eval("<", _lt);
    ret->define_eval("<=", _lte);
    ret->define_eval("==", _eq);
    ret->define_eval("!=", _neq);
    ret->define_eval("*", _mul);
    ret->define_eval("+", _add);
    ret->define_eval("/", _div);
    ret->define_eval("-", _sub);
    ret->define_eval("uint64", _uint64);
    ret->define_eval("int32", _int32);
    ret->define_eval("int16", _int16);
    ret->define_eval("int64", _int64);
    ret->define_eval("uint16", _uint16);
    ret->define_eval("uint32", _uint32);
    ret->define_eval("int8", _int8);
    ret->define_eval("uint8", _uint8);
    ret->define_eval("!", _not);

}
