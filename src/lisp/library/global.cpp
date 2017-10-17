#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


instance<Scope> lisp::make_library_globals(instance<Environment> env)
{
	auto ret = instance<Scope>::make(env, instance<>());

	// -- Compiler Specials --
	auto truth = instance<MultiMethod>::make();
	truth->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(args[0]);
		return instance<bool>::make(*a != 0);
	}
	));
	ret->def("truth", truth);

	// -- Special Forms --
	auto cond = instance<SpecialForm>::make(
		[](instance<Scope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size > 0);
		size -= 1;

		// Do loop of conditions:
		auto truth_subroutine = scope->lookup("truth").asFeature<PSubroutine>();
		size_t index;
		for (index = 1; index < size; index += 2)
		{
			auto branch_result = scope->environment()->eval(sexpr->cells[index], scope);
			auto truth_value = truth_subroutine->call(truth_subroutine, scope, { branch_result });
			if (truth_value.typeId().isType<bool>() && *truth_value.asType<bool>())
			{
				return scope->environment()->eval(sexpr->cells[index + 1], scope);
			}
		}

		// Check for last branch
		if (index - 1 < size)
			return scope->environment()->eval(sexpr->cells[index], scope);
		else
			return instance<>();
	});
	ret->def("cond", cond);

	auto define = instance<SpecialForm>::make(
		[](instance<Scope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		assert(sexpr->cells.size() == 3);

		auto name = sexpr->cells[1];
		auto object = sexpr->cells[2];

		std::string name_value;
		if (name.typeId().isType<Symbol>())
			name_value = name.asType<Symbol>()->value;
		else if (name.typeId().isType<Keyword>())
			name_value = name.asType<Keyword>()->value;
		else
		{
			name = scope->environment()->eval(name, scope);

			if (name.typeId().isType<Symbol>())
				name_value = name.asType<Symbol>()->value;
			else if (name.typeId().isType<Keyword>())
				name_value = name.asType<Keyword>()->value;
			else if (name.typeId().isType<std::string>())
				name_value = *name.asType<std::string>();
			else
				name_value = name.toString();
		}

		object = scope->environment()->eval(object, scope);

		scope->def(name_value, object);
		return object;
	});
	ret->def("define", define);

	// -- Builtin Library --
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