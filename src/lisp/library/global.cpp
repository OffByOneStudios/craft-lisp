#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

#include "system/math.h"
#include "system/string.h"
#include "system/shim.h"
#include "system/fs.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library::helper;


bool special::helper::truth(instance<SFrame> frame, instance<PSubroutine> truth, instance<> code)
{
	auto result = frame->environment()->eval(frame, code);
	auto value = truth->call(truth, frame, { result });

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

instance<SubroutineSignature> library::helper::binding_expr_to_signature(instance<SScope> scope, instance<Sexpr> expr)
{
	// (foo bar baz)
	// ((foo Int) bar (baz (Union String Int)))
	// (foo Int, bar, baz (Union String Int))
	// TODO:
	// * optional
	//   ? ((foo Int) (bar Int 8))
	//   ? ((foo Int) (bar Int :optional))
	// * keyword arugments
	//   ? ((foo Int) (:bar Int) (:baz 8))
	//   ? ((foo Int) (:keyword bar Int) (:keyword baz 8))
	// * return type
	//   ? ((foo Int) (bar Int 8) (:baz 8) Int)
	//   ? ((foo Int) (bar Int 8) (:baz 8) (:returns Int))
	// * collections
	//   ? ((foo Int) (args :args))
	//   ? ((foo Int) (args :args) (kwargs :keywords))

	auto env = scope->environment();
	auto ret = instance<SubroutineSignature>::make();

	for (auto cell : expr->cells)
	{
		auto arg = instance<Argument>::make();

		if (cell.typeId().isType<Sexpr>())
		{
			auto arg_expr = cell.asType<Sexpr>();

			if (arg_expr->cells.size() > 0)
			{
				arg->name = symbol(arg_expr->car());
			}
			if (arg_expr->cells.size() > 1)
			{
				// TODO build type expression
				auto next = env->read(scope, arg_expr->cells[1]);
				next = env->read_eval(scope, next);

				if (!next.hasFeature<types::SType>())
					throw stdext::exception("Signature expected type object.");
				arg->type = next;
			}
			else
			{
				arg->type = env->type_any();
			}
		}
		else
		{
			arg->name = symbol(cell);
			arg->type = env->type_any();
		}

		ret->arguments.push_back(arg);
	}

	ret->complete();
	return ret;
}

instance<Module> lisp::make_library_globals(instance<Namespace> ns)
{
	auto env = ns->environment();
	auto ret = instance<Module>::make(ns, "builtin://globals");

	// -- Types --
	auto Any = instance<types::Special>::make();
	Any->kind = types::Special::Any;
	ret->define_eval("Any", Any);

	auto Bottom = instance<types::Special>::make();
	Bottom->kind = types::Special::Bottom;
	ret->define_eval("Bottom", Bottom);

	auto String = instance<lisp::types::CraftType>::make(types::type<std::string>::typeId());
	ret->define_eval("String", String);

	auto Bool = instance<lisp::types::CraftType>::make(types::type<bool>::typeId());
	ret->define_eval("Bool", Bool);

	auto Int = instance<lisp::types::CraftType>::make(types::type<int64_t>::typeId());
	ret->define_eval("Int", Int);

	auto Float64 = instance<lisp::types::CraftType>::make(types::type<double>::typeId());
	ret->define_eval("Float64", Float64);

	auto Symbol_ = instance<lisp::types::CraftType>::make(types::type<lisp::Symbol>::typeId());
	ret->define_eval("Symbol", Symbol_);

	auto Keyword_ = instance<lisp::types::CraftType>::make(types::type<lisp::Keyword>::typeId());
	ret->define_eval("Keyword", Keyword_);

	auto Sexpr_ = instance<lisp::types::CraftType>::make(types::type<lisp::Sexpr>::typeId());
	ret->define_eval("Sexpr", Sexpr_);

	auto craft_type = instance<BuiltinFunction>::make(
		[](instance<SFrame> frame, std::vector<instance<>> args) -> instance<>
	{
		auto pidentifier = types::system().getManager<PIdentifier>();

		if (args.size() == 0)
		{
			for (auto tid : pidentifier->supportedTypes())
				frame->environment()->log()->info(tid.toString());
			return instance<>();
		}

		auto lookup = symbol(args[0]);

		auto type = pidentifier->index(lookup);

		if (type == types::None)
			return instance<>();

		return instance<lisp::types::CraftType>::make(type);
	});
	ret->define_eval("craft-type", craft_type);

	auto Tuple = instance<BuiltinFunction>::make(
		[](auto frame, auto args) -> instance<>
	{
		auto ret = instance<lisp::types::Tuple>::make();

		for (auto it = args.begin(); it != args.end(); ++it)
		{
			//assert(it->hasFeature<::craft::lisp::types::SType>());
			ret->cells.push_back(*it);
		}

		return ret;
	});
	ret->define_eval("Tuple", Tuple);

	auto Union = instance<BuiltinFunction>::make(
		[](auto frame, auto args) -> instance<>
	{
		auto ret = instance<lisp::types::Union>::make();

		for (auto it = args.begin(); it != args.end(); ++it)
		{
			//assert(it->hasFeature<lisp::types::SType>());
			ret->variants.push_back(*it);
		}

		return ret;
	});
	ret->define_eval("Union", Union);

	auto subtype = instance<MultiMethod>::make();
	subtype->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<types::SType, types::SType, bool>(),
		[](instance<SFrame> frame, std::vector<instance<>> const& args) -> instance<>
	{
		instance<> a(args[0]), b(args[1]);

		return instance<bool>::make(types::is_subtype(frame->environment(), a, b));
	}));
	ret->define_eval("subtype?", subtype);
	ret->define_eval("\u227C", subtype);

	// -- Compiler Specials --
	auto truth = instance<MultiMethod>::make();
	truth->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, std::vector<instance<>> const& args)
	{
		instance<int64_t> a(args[0]);
		return instance<bool>::make(*a != 0);
	}));
	ret->define_eval("truth", truth);
	ret->define_eval("?", truth);

	// -- Special Forms --
	auto do_ = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		return scope->environment()->read_rest(scope, head, sexpr);
	},
		[](instance<SFrame> frame, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();

		instance<> last_ret;
		for (int i = 0; i < size; i += 1)
		{
			last_ret = frame->environment()->eval(frame, sexpr->cells[i]);
		}

		return last_ret;
	});
	ret->define_eval("do", do_);

	auto define = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		if (sexpr->cells.size() != 3)
			throw stdext::exception("malformed: (define <symbol> <expr>)");

		auto name = sexpr->cells[1];
		auto object = sexpr->cells[2];

		std::string name_value = symbol(name);

		object = scope->environment()->read(scope, object);

		auto binding = scope->define(name_value, object);

		auto ret = instance<Sexpr>::make();
		ret->cells.push_back(head);
		ret->cells.push_back(binding);
		return ret;
	},
		[](instance<SFrame> frame, instance<Sexpr> sexpr) -> instance<>
	{
		if (sexpr->cells.size() != 2 || !sexpr->cells[1].typeId().hasFeature<SBinding>())
			throw stdext::exception("Malformed define evaluated.");

		instance<SBinding> binding = sexpr->cells[1];

		binding->eval(frame);

		return binding->getValue(frame);
	});
	ret->define_eval("define", define);

	auto cond = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		return scope->environment()->read_rest(scope, head, sexpr);
	},
		[truth](instance<SFrame> frame, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size > 0);
		size -= 1;

		// Do loop of conditions:
		size_t index;
		for (index = 1; index < size; index += 2)
		{
			if (special::helper::truth(frame, truth, sexpr->cells[index]))
				return frame->environment()->eval(frame, sexpr->cells[index + 1]);
		}

		// Check for last branch
		if (index - 1 < size)
			return frame->environment()->eval(frame, sexpr->cells[index]);
		else
			return instance<>();
	});
	ret->define_eval("cond", cond);

	auto _while = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		return scope->environment()->read_rest(scope, head, sexpr);
	},
		[truth](instance<SFrame> frame, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size == 3);
		size -= 1;

		auto condition = sexpr->cells[1];
		auto body = sexpr->cells[2];

		instance<> last_ret;
		while (special::helper::truth(frame, truth, condition))
		{
			last_ret = frame->environment()->eval(frame, body);
		}

		return last_ret;
	});
	ret->define_eval("while", _while);

	auto fn = instance<SpecialForm>::make(
		[](instance<SScope> scope, instance<> head, instance<Sexpr> sexpr) -> instance<>
	{
		// Setup special form
		size_t size = sexpr->cells.size();
		assert(size == 3);
		size -= 1;

		auto binding = sexpr->cells[1];
		auto body = sexpr->cells[2];

		auto function = instance<Function>::make();

		auto signature = library::helper::binding_expr_to_signature(scope, binding);

		function->setSignature(signature);

		auto func_scope = signature->read_frame(scope);
		body = func_scope->environment()->read(func_scope, body);

		function->setBody(body);

		auto ret = instance<Sexpr>::make();
		ret->cells.push_back(head);
		ret->cells.push_back(function);
		return ret;
	},
		[](instance<SFrame> frame, instance<Sexpr> sexpr) -> instance<>
	{
		if (sexpr->cells.size() != 2 || !sexpr->cells[1].typeId().isType<Function>())
			throw stdext::exception("Malformed fn evaluated.");

		auto function = sexpr->cells[1];

		return instance<Closure>::make(frame, function);
	});
	ret->define_eval("fn", fn);

	// -- Builtin Library --
	auto lookup = instance<MultiMethod>::make();
	lookup->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<SScope> lookup_scope(args[0]);
		auto lookup_name = symbol(args[1]);

		auto binding = lookup_scope->lookup(lookup_name);
		if (binding)
			return binding->getValue(frame);

		return instance<>();
	}));
	ret->define_eval("lookup", lookup);

	auto lookup_meta = instance<MultiMethod>::make();
	lookup_meta->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<SScope> lookup_scope(args[0]);
		auto lookup_name = symbol(args[1]);
		auto lookup_meta = symbol(args[2]);

		auto binding = lookup_scope->lookup(lookup_name);
		if (binding)
			return binding->getMeta(lookup_meta);

		return instance<>();
	}));
	ret->define_eval("lookup/meta", lookup_meta);

	// Quick Maths
	system::make_math_globals(ret, ns);
	system::make_string_globals(ret, ns);
	system::make_shim_globals(ret, ns);
	system::make_fs_globals(ret, ns);


	auto file_text = instance<MultiMethod>::make();
	file_text->attach(env, instance<BuiltinFunction>::make(
		[](instance<SFrame> frame, auto args)
	{
		auto s = path::normalize(path::absolute(*args[0].asType<std::string>()));
		auto text = craft::fs::read<std::string>(s, &craft::fs::string_read).get();

		return instance<std::string>::make(text);
	}));
	ret->define_eval("file/text", file_text);

	auto file_eval = instance<MultiMethod>::make();
	file_eval->attach(env, instance<BuiltinFunction>::make(
		[=](instance<SFrame> frame, auto args)
	{
		auto s = path::normalize(path::absolute(*args[0].asType<std::string>()));
		auto module = instance<Module>::make(ns, fmt::format("file:{0}", s));

		module->load();

		return module;
	}));
	ret->define_eval("file/eval", file_eval);

	//
	// MultiMethod
	//
	auto MultiMethod_ = instance<MultiMethod>::make();
	MultiMethod_->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		return instance<MultiMethod>::make();
	}));
	ret->define_eval("MultiMethod", MultiMethod_);

	auto attach = instance<lisp::MultiMethod>::make();
	attach->attach(env, instance<BuiltinFunction>::make(
		[](instance<SFrame> frame, auto args)
	{
		instance<MultiMethod> a(args[0]); instance<> b(args[1]);

		a->attach(frame->environment(), b);

		return a;
	}));
	ret->define_eval("attach", attach);

	auto dispatch = instance<lisp::MultiMethod>::make();
	dispatch->attach(env, instance<BuiltinFunction>::make(
		[](instance<SFrame> frame, auto args)
	{
		instance<MultiMethod> a(args[0]); instance<> b(args[1]);

		MultiMethod::Dispatch _dispatch;

		a->dispatch(frame->environment(), b, &_dispatch);

		return _dispatch.subroutine;
	}));
	ret->define_eval("dispatch", dispatch);

	//
	// Variable
	//
	auto Variable_ = instance<MultiMethod>::make();
	Variable_->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		if (args.size() > 0)
			return instance<Variable>::make(args[0]);
		return instance<Variable>::make();
	}));
	ret->define_eval("Variable", Variable_);

	auto get = instance<lisp::MultiMethod>::make();
	get->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Variable>(),
		[](auto frame, auto args)
	{
		instance<Variable> var(args[0]);
		return var->get();
	}));
	ret->define_eval("get", get);

	auto set = instance<lisp::MultiMethod>::make();
	set->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<Variable> var(args[0]); instance<> value(args[1]);
		var->set(value);

		return instance<>();
	}));
	ret->define_eval("set", set);

	//
	// Introspection
	//

	auto report = instance<MultiMethod>::make();
	report->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Function>(),
		[](auto frame, auto args)
	{
		instance<Function> a(expect<Function>(args[0]));

		return instance<std::string>::make("");
	}));
	ret->define_eval("report", report);

	auto time_highres = instance<MultiMethod>::make();
	time_highres->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		return instance<std::int64_t>::make((int64_t)(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	}));
	ret->define_eval("time-highres", time_highres);

	auto timeit = instance<Macro>::make(
		[](instance<SScope> scope, std::vector<instance<>> const& code)
	{
		instance<Sexpr> expr = scope->environment()->parse(scope, "(do (define g_xxxx_begin (time-highres)) :replace (define g_xxxx_end (time-highres)) (- g_xxxx_end g_xxxx_begin))");

		expr->cells[0].asType<Sexpr>()->cells[2] = code[1];

		return expr->cells[0];
	});
	ret->define_eval("timeit", timeit);
	
	return ret;
}
