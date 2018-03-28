
#include "lisp/common.h"
#include "lisp/library/libraries.h"


using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void multdiv(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto mul = instance<MultiMethod>::make();
	mul->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a * *b);
	}));
	mul->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<double>::make(*a * *b);
	}));
	ret->define_eval("*", mul);

	auto div = instance<MultiMethod>::make();
	div->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a / *b);
	}));
	div->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<double>::make(*a / *b);
	}));
	ret->define_eval("/", div);
}

void addsub(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto add = instance<MultiMethod>::make();
	add->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a + *b);
	}));
	add->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<double>::make(*a + *b);
	}));
	ret->define_eval("+", add);


	auto sub = instance<MultiMethod>::make();
	sub->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, int64_t>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a - *b);
	}));
	sub->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, double>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<double>::make(*a - *b);
	}));
	ret->define_eval("-", sub);
}

void logic(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();
	
	auto lt = instance<MultiMethod>::make();
	lt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<bool>::make(*a < *b);
	}));
	lt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<bool>::make(*a < *b);
	}));
	ret->define_eval("<", lt);

	auto lteq = instance<MultiMethod>::make();
	lteq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<bool>::make(*a <= *b);
	}));
	lteq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<bool>::make(*a <= *b);
	}));
	ret->define_eval("<=", lteq);

	auto eq = instance<MultiMethod>::make();
	eq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<bool>::make(*a == *b);
	}));
	eq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<bool>::make(*a == *b);
	}));
	eq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
		[](auto frame, auto args)
	{
		instance<bool> a(expect<bool> (args[0])), b(expect<bool>(args[1]));
		return instance<bool>::make(*a == *b);
	}));
	eq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string, bool>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));
		return instance<bool>::make(*a == *b);
	}));
	ret->define_eval("==", eq);

	auto neq = instance<MultiMethod>::make();
	neq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<bool>::make(*a != *b);
	}));
	neq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<bool>::make(*a != *b);
	}));
	neq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<bool, bool, bool>(),
		[](auto frame, auto args)
	{
		instance<bool> a(expect<bool>(args[0])), b(expect<bool>(args[1]));
		return instance<bool>::make(*a != *b);
	}));
	neq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string, bool>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));
		return instance<bool>::make(*a == *b);
	}));
	ret->define_eval("!=", neq);

	auto gteq = instance<MultiMethod>::make();
	gteq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<bool>::make(*a >= *b);
	}));
	gteq->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<bool>::make(*a >= *b);
	}));
	ret->define_eval(">=", gteq);

	auto gt = instance<MultiMethod>::make();
	gt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<bool>::make(*a > *b);
	}));
	gt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, double, bool>(),
		[](auto frame, auto args)
	{
		instance<double> a(expect<double>(args[0])), b(expect<double>(args[1]));
		return instance<bool>::make(*a > *b);
	}));
	ret->define_eval(">", gt);


	auto not = instance<MultiMethod>::make();
	not->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0]));

		return instance<bool>::make(!(*a));
	}));
	not->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<double, bool>(),
		[](auto frame, auto args)
	{
		instance<int64_t> a(expect<double>(args[0]));

		return instance<bool>::make(!(*a));
	}));
	not->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<bool, bool>(),
		[](auto frame, auto args)
	{
		instance<bool> a(expect<bool>(args[0]));

		return instance<bool>::make(!(*a));
	}));
	ret->define_eval("!", not);

}


void system::make_math_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	addsub(ret, ns);
	multdiv(ret, ns);
	logic(ret, ns);
}
