#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

bool special::helper::truth(instance<Scope> scope, instance<PSubroutine> truth, instance<> code)
{
	auto result = scope->environment()->eval(code, scope);
	auto value = truth->call(truth, scope, { result });

	assert(value.typeId().isType<bool>());

	return *value.asType<bool>();
}

instance<Scope> lisp::make_library_globals(instance<Environment> env)
{
	auto ret = instance<Scope>::make(env, instance<>());

	// -- Types --
	auto Any = instance<types::Special>::make();
	Any->kind = types::Special::Any;
	ret->def("Any", Any);

	auto Bottom = instance<types::Special>::make();
	Bottom->kind = types::Special::Bottom;
	ret->def("Bottom", Bottom);

	auto craft_type = instance<BuiltinFunction>::make(
		[](auto scope, auto args) -> instance<>
	{
		instance<> arg(args[0]);
		std::string lookup;
		
		if (arg.typeId().isType<Keyword>())
			lookup = arg.asType<Keyword>()->value;
		else
			lookup = arg.toString();

		auto type = types::system().getManager<PIdentifier>()->index(lookup);
		if (type == types::None)
			return instance<>();

		return instance<lisp::types::CraftType>::make(type);
	});
	ret->def("craft-type", craft_type);

	auto subtype = instance<MultiMethod>::make();
	subtype->attach(env, instance<BuiltinFunction>::make(
		[](instance<Scope> scope, auto args) -> instance<>
	{
		instance<> a(args[0]), b(args[1]);

		types::AlgorithmSubtype* algo = new types::AlgorithmSubtype(scope->environment(), a, b);

		algo->execute();
		return instance<bool>::make(algo->leftIsSubtype);
	}));
	ret->def("subtype", subtype);

	// -- Compiler Specials --
	auto truth = instance<MultiMethod>::make();
	truth->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(args[0]);
		return instance<bool>::make(*a != 0);
	}));
	ret->def("truth", truth);

	// -- Special Forms --
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
			if (special::helper::truth(scope, truth_subroutine, sexpr->cells[index]))
				return scope->environment()->eval(sexpr->cells[index + 1], scope);
		}

		// Check for last branch
		if (index - 1 < size)
			return scope->environment()->eval(sexpr->cells[index], scope);
		else
			return instance<>();
	});
	ret->def("cond", cond);

	auto _while = instance<SpecialForm>::make(
		[](instance<Scope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size == 3);
		size -= 1;

		auto condition = sexpr->cells[1];
		auto body = sexpr->cells[2];

		auto truth_subroutine = scope->lookup("truth").asFeature<PSubroutine>();

		instance<> last_ret;
		while (special::helper::truth(scope, truth_subroutine, condition))
		{
			last_ret = scope->environment()->eval(body, scope);
		}

		return last_ret;
	});
	ret->def("while", _while);

	// -- Builtin Library --
	auto add = instance<MultiMethod>::make();
	add->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(args[0]), b(args[1]);
		return instance<int64_t>::make(*a + *b);
	}));
	ret->def("+", add);

	auto sub = instance<MultiMethod>::make();
	sub->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(args[0]), b(args[1]);
		return instance<int64_t>::make(*a - *b);
	}));
	ret->def("-", sub);

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