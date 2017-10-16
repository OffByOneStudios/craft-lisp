#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


instance<Scope> lisp::make_library_globals(instance<Environment> env)
{
	auto ret = instance<Scope>::make(env, instance<>());

	auto add = instance<MultiMethod>::make();
	add->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(args[0]), b(args[1]);
		return instance<int64_t>::make(*a + *b);
	}
	));
	ret->def("+", add);

	/*
	auto car = instance<MultiMethod>::make();
	car->attach(instance<BuiltinFunction>::make(
	[](auto scope, auto args)
	{
	instance<int64_t> a(args[0].);
	return instance<Sexpr>::make(*a)->car();
	}
	));
	global->def("car", car);

	auto cdr = instance<MultiMethod>::make();
	cdr->attach(instance<BuiltinFunction>::make(
	[](auto scope, auto args)
	{
	instance<int64_t> a(args[0]), b(args[1]);
	return instance<int64_t>(*a + *b);
	}
	));
	global->def("cdr", cdr);
	*/

	return ret;
}