#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


typedef instance<Scope> iScope;
typedef std::vector<instance<>> tArgs;

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

	auto String = instance<lisp::types::CraftType>::make(types::type<std::string>::typeId());
	ret->def("String", String);

	auto Bool = instance<lisp::types::CraftType>::make(types::type<bool>::typeId());
	ret->def("Bool", Bool);

	auto craft_type = instance<BuiltinFunction>::make(
		[](iScope scope, tArgs args) -> instance<>
	{
		auto pidentifier = types::system().getManager<PIdentifier>();

		if (args.size() == 0)
		{
			for (auto tid : pidentifier->supportedTypes())
				scope->environment()->log()->info(tid.toString());
			return instance<>();
		}

		instance<> arg(args[0]);
		std::string lookup;

		if (arg.typeId().isType<Keyword>())
			lookup = arg.asType<Keyword>()->value;
		else
			lookup = arg.toString();

		auto type = pidentifier->index(lookup);

		if (type == types::None)
			return instance<>();

		return instance<lisp::types::CraftType>::make(type);
	});
	ret->def("craft-type", craft_type);

	auto Tuple = instance<BuiltinFunction>::make(
		[](auto scope, auto args) -> instance<>
	{
		auto ret = instance<lisp::types::Tuple>::make();

		for (auto it = args.begin(); it != args.end(); ++it)
		{
			//assert(it->hasFeature<::craft::lisp::types::SType>());
			ret->cells.push_back(*it);
		}

		return ret;
	});
	ret->def("Tuple", Tuple);

	auto Union = instance<BuiltinFunction>::make(
		[](auto scope, auto args) -> instance<>
	{
		auto ret = instance<lisp::types::Union>::make();

		for (auto it = args.begin(); it != args.end(); ++it)
		{
			//assert(it->hasFeature<lisp::types::SType>());
			ret->variants.push_back(*it);
		}

		return ret;
	});
	ret->def("Union", Union);

	auto subtype = instance<MultiMethod>::make();
	subtype->attach(env, instance<BuiltinFunction>::make(
		[](instance<Scope> scope, auto args) -> instance<>
	{
		instance<> a(args[0]), b(args[1]);

		types::AlgorithmSubtype* algo = new types::AlgorithmSubtype(scope->environment(), a, b);

		algo->execute_subtype();
		return instance<bool>::make(algo->leftIsSubtype);
	}));
	ret->def("subtype?", subtype);
	ret->def("\u227C", subtype);

	// -- Compiler Specials --
	auto truth = instance<MultiMethod>::make();
	truth->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(args[0]);
		return instance<bool>::make(*a != 0);
	}));
	ret->def("truth", truth);
	ret->def("?", truth);

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

	auto lambda = instance<SpecialForm>::make(
		[](instance<Scope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size == 3);
		size -= 1;

		auto binding = sexpr->cells[1];
		auto body = sexpr->cells[2];

		auto function = instance<Function>::make();
		function->setBinding(binding);
		function->setBody(body);

		return function;
	});
	ret->def("lambda", lambda);

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

	auto eval_file = instance<MultiMethod>::make();
	eval_file->attach(env, instance<BuiltinFunction>::make(
		[](iScope scope, auto args)
	{
		try
		{
			auto s = path::normalize(path::absolute(*args[0].asType<std::string>()));
			auto ext = path::extname(s);
			if (ext != "cult")
			{
				throw stdext::exception("Scripts must be of extension .cult");
			}
			auto script = craft::fs::read<std::string>(s, &craft::fs::string_read).get();

			auto env = scope->environment();
			auto cell = env->read(script);
			for (auto c : cell->cells)
			{
				env->eval(c, scope);
			}

			return instance<>();
		}
		catch (stdext::exception e)
		{
			return (instance<>)instance<std::string>::make(e.what());
		}
	}));
	ret->def("eval-file", eval_file);

	auto MultiMethod_ = instance<MultiMethod>::make();
	MultiMethod_->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		return instance<MultiMethod>::make();
	}));
	ret->def("MultiMethod", MultiMethod_);

	auto attach = instance<lisp::MultiMethod>::make();
	attach->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<MultiMethod> a(args[0]); instance<> b(args[1]);
		return instance<lisp::MultiMethod>::make();
	}));
	ret->def("attach", attach);

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
