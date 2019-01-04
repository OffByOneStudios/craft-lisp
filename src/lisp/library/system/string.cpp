
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

#define bMM semantics->builtin_implementMultiMethod
#define st "string"

#ifndef ssize_t
#define ssize_t int64_t
#endif

typedef instance<std::string> t_str;

void core::make_string_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	bMM(st, [](t_str a) -> t_str { return t_str::make(*a);});
	bMM(st, [](instance<PStringer> a) -> t_str { return t_str::make(a.getFeature<PStringer>()->toString(a)); });
	bMM(st, [](instance<PRepr> a) -> t_str { return t_str::make(a.getFeature<PRepr>()->toRepr(a)); });
	bMM(st, [](instance<> a) -> t_str { 
		if (a.isNull()) return t_str::make("null");
		return t_str::make(a.toString());
	});

	bMM("print", [](types::VarArgs<instance<>> args) {
		for (auto& it : args.args)
		{
			std::cout << *Execution::exec_fromCurrentModule(st, { it }).asType<std::string>();
			std::cout << " ";
		}
		std::cout << "\n";
	});

	bMM("format", [](t_str fmt, types::VarArgs<instance<>> args) -> t_str {
		std::regex re("%\\{[^\\}]+\\}");
		std::ostringstream s;

		std::sregex_token_iterator
			begin(fmt->begin(), fmt->end(), re, { -1, 0 }),
			end;

		std::for_each(begin, end,
			[&](std::string const& m)
		{
			if (std::regex_match(m, re))
			{
				std::string match = m.substr(2, m.size() - 3);
				instance<> target;

				ssize_t i = -1;
				try
				{
					i = std::stoi(match);
				}
				catch (...)
				{

				}
				if (i != -1)
				{
					if (size_t(i) >= args.args.size())
					{
						throw stdext::exception("Argument Reference {0} Out of Bounds", i);
					}
					target = args.args[i];
				}
				else
				{
					throw stdext::exception("TODO string formatter literate execution is disabled");
					target = Execution::exec_fromCurrentModule(match, {});
				}

				t_str c = Execution::exec_fromCurrentModule(st, { target });
				s << *c;
			}
			else
			{
				s << m;
			}
		});

		return t_str::make(s.str());
	});

	bMM("log", [](instance<Symbol> sym, t_str str, types::VarArgs<instance<>> args)
	{
		auto size = args.args.size() + 1;
		if (size != 1)
		{
			GenericInvoke invoke(size);
			invoke.args.push_back(str); std::copy(args.args.begin(), args.args.end(), std::back_inserter(invoke.args));
			str = Execution::exec_fromCurrentModule("format", invoke).asType<std::string>();
		}

		auto env = Execution::getCurrent()->getEnvironment();

		spdlog::level::level_enum level = spdlog::level::level_enum::off;
		if (sym->isKeyword())
		{
			auto critical = env->symbolStore.intern("critical");
			if (critical == sym->at(1)) level = spdlog::level::level_enum::critical;
			auto error = env->symbolStore.intern("error");
			if (error == sym->at(1)) level = spdlog::level::level_enum::err;
			auto warning = env->symbolStore.intern("warning");
			if (warning == sym->at(1)) level = spdlog::level::level_enum::warn;
			auto info = env->symbolStore.intern("info");
			if (info == sym->at(1)) level = spdlog::level::level_enum::info;
			auto debug = env->symbolStore.intern("debug");
			if (debug == sym->at(1)) level = spdlog::level::level_enum::debug;
			auto trace = env->symbolStore.intern("trace");
			if (trace == sym->at(1)) level = spdlog::level::level_enum::trace;
		}

		if (level == spdlog::level::level_enum::off)
			throw stdext::exception("Unknown level {0}", sym->getDisplay());

		env->log()->log(level, str->c_str());
	});

	bMM("log", [](instance<Symbol> sym, instance<> thing)
	{
		auto str = Execution::exec_fromCurrentModule(st, { thing }).asType<std::string>();
		Execution::exec_fromCurrentModule("format", { sym, str }).asType<std::string>();
	});

	bMM(st"/concat", [](t_str a, t_str b) -> t_str { return t_str::make(fmt::format("{0}{1}", *a, *b)); });

	bMM(st"/split",
		[](t_str a, t_str b) -> instance<List>
	{
		std::vector<std::string> sm;
		stdext::split(*a, *b, std::back_inserter(sm));

		std::vector<instance<>> res;
		for (auto i : sm)
		{
			res.push_back(t_str::make(i));
		}

		return instance<List>::make(res);
	});

	bMM(st"/join",
		[](instance<List> a, t_str b) -> t_str
	{
		//std::cout << *Execution::exec_fromCurrentModule(st, { a }).asType<std::string>();

		std::ostringstream res;

		auto count = 0;
		for (auto& i : a->data())
		{
			res << *Execution::exec_fromCurrentModule(st, { i }).asType<std::string>();
			if (count != (*a->size()) - 1) res << *b;
			count++;
		}

		return t_str::make(res.str());
	});

	bMM(st"/join",
		[](instance<List> a) -> t_str
	{
		return Execution::exec_fromCurrentModule(st"/join", { a, t_str::make("\n") }).asType<std::string>();
	});


	bMM(st"/slice",
		[](t_str a, instance<int64_t> b, instance<int64_t> c) -> t_str
	{
		return t_str::make(a->substr(*b, *c));
	});

	bMM(st"/reverse",
		[](t_str a) -> t_str
	{
		auto res = t_str::make(*a);
		std::reverse(res->begin(), res->end());
		return res;
	});

	bMM("string/includes",
		[](t_str a, t_str b) -> instance<bool>
	{
		return instance<bool>::make(a->find(*b) != a->npos);
	});

	bMM("string/compare",
		[](t_str a, t_str b) -> instance<int64_t>
	{
		return instance<int64_t>::make(std::strcmp(a->c_str(), b->c_str()));
	});

	bMM("string/isprefix",
		[](t_str a, t_str b) -> instance<bool>
	{
		bool res;
		if (a->size() > b->size()) res = false;
		else
		{
			res = *a == b->substr(0, a->size());
		}

		return instance<bool>::make(res);
	});

	bMM("string/length", [](t_str a) { return instance<uint64_t>::make(a->size());});


	bMM("string/upper", [](t_str a) { 
		auto res = t_str::make(*a);
		std::transform(res->begin(), res->end(), res->begin(), ::toupper);
		return res;
	});

	bMM("string/lower", [](t_str a) {
		auto res = t_str::make(*a);
		std::transform(res->begin(), res->end(), res->begin(), ::tolower);
		return res;
	});

	bMM("string/capital", [](t_str a) {
		auto res = t_str::make(*a);
		std::transform(res->begin(), res->begin() + 1, res->begin(), ::toupper);
		return res;
	});

	bMM("string/snake", [](t_str a) { return t_str::make(std::regex_replace(*a, std::regex(" "), "_")); });

	bMM("string/kebab", [](t_str a) { return t_str::make(std::regex_replace(*a, std::regex(" "), "-")); });

	bMM("string/camel", [](t_str a) {
		std::ostringstream res;
		std::vector<std::string> sm;
		stdext::split(*a, " ", std::back_inserter(sm));
		sm[0][0] = ::tolower(sm[0][0]);;
		res << sm[0];
		for (auto i = 1; i < sm.size(); i++)
		{
			sm[i][0] = ::toupper(sm[i][0]);
			res << sm[i];
		}
		return t_str::make(res.str());
	});
}
