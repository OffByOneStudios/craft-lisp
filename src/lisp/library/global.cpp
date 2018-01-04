#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library::helper;


bool special::helper::truth(instance<SScope> scope, instance<PSubroutine> truth, instance<> code)
{
	auto result = scope->environment()->eval(scope, code);
	auto value = truth->call(truth, scope, { result });

	assert(value.typeId().isType<bool>());

	return *value.asType<bool>();
}

std::string library::helper::symbol(instance<> s)
{
	if (s.typeId().isType<Symbol>())
		return s.asType<Symbol>()->value;
	else if (s.typeId().isType<Keyword>())
		return s.asType<Keyword>()->value;
	else if (s.typeId().isType<std::string>())
		return *s.asType<std::string>();
	else
		throw stdext::exception("The given {0} cannot be used as a symbol", s.typeId().toString());
}

instance<Module> lisp::make_library_globals(instance<Namespace> ns)
{
	auto env = ns->environment();
	auto ret = instance<Module>::make(ns, "builtin://globals");

	// -- Types --
	auto Any = instance<types::Special>::make();
	Any->kind = types::Special::Any;
	ret->define(instance<Binding>::make("Any", Any));

	auto Bottom = instance<types::Special>::make();
	Bottom->kind = types::Special::Bottom;
	ret->define(instance<Binding>::make("Bottom", Bottom));

	auto String = instance<lisp::types::CraftType>::make(types::type<std::string>::typeId());
	ret->define(instance<Binding>::make("String", String));

	auto Bool = instance<lisp::types::CraftType>::make(types::type<bool>::typeId());
	ret->define(instance<Binding>::make("Bool", Bool));

	auto Int = instance<lisp::types::CraftType>::make(types::type<int64_t>::typeId());
	ret->define(instance<Binding>::make("Int", Int));

	auto Float64 = instance<lisp::types::CraftType>::make(types::type<double>::typeId());
	ret->define(instance<Binding>::make("Float64", Float64));

	auto craft_type = instance<BuiltinFunction>::make(
		[](instance<SScope> scope, std::vector<instance<>> args) -> instance<>
	{
		auto pidentifier = types::system().getManager<PIdentifier>();

		if (args.size() == 0)
		{
			for (auto tid : pidentifier->supportedTypes())
				scope->environment()->log()->info(tid.toString());
			return instance<>();
		}

		auto lookup = symbol(args[0]);

		auto type = pidentifier->index(lookup);

		if (type == types::None)
			return instance<>();

		return instance<lisp::types::CraftType>::make(type);
	});
	ret->define(instance<Binding>::make("craft-type", craft_type));

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
	ret->define(instance<Binding>::make("Tuple", Tuple));

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
	ret->define(instance<Binding>::make("Union", Union));

	auto subtype = instance<MultiMethod>::make();
	subtype->attach(env, instance<BuiltinFunction>::make(
		[](instance<SScope> scope, std::vector<instance<>> const& args) -> instance<>
	{
		instance<> a(args[0]), b(args[1]);

		types::AlgorithmSubtype* algo = new types::AlgorithmSubtype(scope->environment(), a, b);

		algo->execute_subtype();
		return instance<bool>::make(algo->leftIsSubtype);
	}));
	ret->define(instance<Binding>::make("subtype?", subtype));
	ret->define(instance<Binding>::make("\u227C", subtype));

	// -- Compiler Specials --
	auto truth = instance<MultiMethod>::make();
	truth->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, std::vector<instance<>> const& args)
	{
		instance<int64_t> a(args[0]);
		return instance<bool>::make(*a != 0);
	}));
	ret->define(instance<Binding>::make("truth", truth));
	ret->define(instance<Binding>::make("?", truth));

	// -- Special Forms --
	auto define = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		if (sexpr->cells.size() != 3)
			throw stdext::exception("malformed: (define <symbol> <expr>)");

		auto name = sexpr->cells[1];
		auto object = sexpr->cells[2];

		std::string name_value;

		if (name.typeId().isType<Sexpr>())
			name = scope->environment()->eval(scope, name);

		name_value = symbol(name);

		object = scope->environment()->read(scope, object);

		auto binding = instance<Binding>::make(name_value, object);
		scope->define(binding);

		auto ret = instance<Sexpr>::make();
		ret->cells.push_back(head);
		ret->cells.push_back(binding);
		return ret;
	},
		[](instance<SScope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		if (sexpr->cells.size() != 2 || !sexpr->cells[1].typeId().isType<Binding>())
			throw stdext::exception("Malformed define evaluated.");

		instance<Binding> binding = sexpr->cells[1];

		binding->eval(scope);

		return binding->value();
	});
	ret->define(instance<Binding>::make("define", define));

	auto cond = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		return scope->environment()->read_rest(scope, head, sexpr);
	},
		[](instance<SScope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size > 0);
		size -= 1;

		// Do loop of conditions:
		auto truth_subroutine = scope->lookup("truth")->value().asFeature<PSubroutine>();
		size_t index;
		for (index = 1; index < size; index += 2)
		{
			if (special::helper::truth(scope, truth_subroutine, sexpr->cells[index]))
				return scope->environment()->eval(scope, sexpr->cells[index + 1]);
		}

		// Check for last branch
		if (index - 1 < size)
			return scope->environment()->eval(scope, sexpr->cells[index]);
		else
			return instance<>();
	});
	ret->define(instance<Binding>::make("cond", cond));

	auto _while = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		return scope->environment()->read_rest(scope, head, sexpr);
	},
		[](instance<SScope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size == 3);
		size -= 1;

		auto condition = sexpr->cells[1];
		auto body = sexpr->cells[2];

		auto truth_subroutine = scope->lookup("truth")->value().asFeature<PSubroutine>();

		instance<> last_ret;
		while (special::helper::truth(scope, truth_subroutine, condition))
		{
			last_ret = scope->environment()->eval(scope, body);
		}

		return last_ret;
	});
	ret->define(instance<Binding>::make("while", _while));

	auto lambda = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size == 3);
		size -= 1;

		auto binding = sexpr->cells[1];
		auto body = sexpr->cells[2];

		auto function = instance<Function>::make();
		function->setBinding(binding);

		body = scope->environment()->read(scope, body);
		function->setBody(body);

		return function;
	},
		[](instance<SScope> scope, instance<Sexpr> sexpr) -> instance<>
	{
		throw stdext::exception("eval'ed lambda");
	});
	ret->define(instance<Binding>::make("lambda", lambda));

	// -- Builtin Library --
	auto lookup = instance<MultiMethod>::make();
	lookup->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<SScope> lookup_scope(args[0]);
		auto lookup_name = symbol(args[1]);

		auto binding = lookup_scope->lookup(lookup_name);
		if (binding)
			return binding->value();

		return instance<>();
	}));
	ret->define(instance<Binding>::make("lookup", lookup));

	auto lookup_meta = instance<MultiMethod>::make();
	lookup_meta->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<SScope> lookup_scope(args[0]);
		auto lookup_name = symbol(args[1]);
		auto lookup_meta = symbol(args[2]);

		auto binding = lookup_scope->lookup(lookup_name);
		if (binding)
			return binding->getMeta(lookup_meta);

		return instance<>();
	}));
	ret->define(instance<Binding>::make("lookup/meta", lookup_meta));

	auto add = instance<MultiMethod>::make();
	add->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a + *b);
	}));
	ret->define(instance<Binding>::make("+", add));

	auto sub = instance<MultiMethod>::make();
	sub->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a - *b);
	}));
	ret->define(instance<Binding>::make("-", sub));

	auto mul = instance<MultiMethod>::make();
	mul->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0])), b(expect<int64_t>(args[1]));
		return instance<int64_t>::make(*a * *b);
	}));
	ret->define(instance<Binding>::make("*", mul));

	auto cwd = instance<MultiMethod>::make();
	cwd->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		return instance<std::string>::make(path::absolute());
	}));
	ret->define(instance<Binding>::make("cwd", cwd));

	auto file_text = instance<MultiMethod>::make();
	file_text->attach(env, instance<BuiltinFunction>::make(
		[](instance<SScope> scope, auto args)
	{
		auto s = path::normalize(path::absolute(*args[0].asType<std::string>()));
		auto text = craft::fs::read<std::string>(s, &craft::fs::string_read).get();

		return instance<std::string>::make(text);
	}));
	ret->define(instance<Binding>::make("file/text", file_text));

	auto file_eval = instance<MultiMethod>::make();
	file_eval->attach(env, instance<BuiltinFunction>::make(
		[=](instance<SScope> scope, auto args)
	{
		auto s = path::normalize(path::absolute(*args[0].asType<std::string>()));
		auto module = instance<Module>::make(ns, fmt::format("file://{0}", s));

		module->load();

		return module;
	}));
	ret->define(instance<Binding>::make("file/eval", file_eval));

	//
	// MultiMethod
	//
	auto MultiMethod_ = instance<MultiMethod>::make();
	MultiMethod_->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		return instance<MultiMethod>::make();
	}));
	ret->define(instance<Binding>::make("MultiMethod", MultiMethod_));

	auto attach = instance<lisp::MultiMethod>::make();
	attach->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<MultiMethod> a(args[0]); instance<> b(args[1]);
		return instance<lisp::MultiMethod>::make();
	}));
	ret->define(instance<Binding>::make("attach", attach));

	//
	// Variable
	//
	auto Variable_ = instance<MultiMethod>::make();
	Variable_->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		if (args.size() > 0)
			return instance<Variable>::make(args[0]);
		return instance<Variable>::make();
	}));
	ret->define(instance<Binding>::make("Variable", Variable_));

	auto get = instance<lisp::MultiMethod>::make();
	get->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<Variable> var(args[0]);
		return var->get();
	}));
	ret->define(instance<Binding>::make("get", get));

	auto set = instance<lisp::MultiMethod>::make();
	set->attach(env, instance<BuiltinFunction>::make(
		[](auto scope, auto args)
	{
		instance<Variable> var(args[0]); instance<> value(args[1]);
		var->set(value);

		return instance<>();
	}));
	ret->define(instance<Binding>::make("set", set));

	return ret;
}
