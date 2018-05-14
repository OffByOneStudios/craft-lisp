
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void core::make_string_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("string",
		[](instance<std::string> a) -> instance<std::string>
	{
		return instance<std::string>::make(*a);
	});
	semantics->builtin_implementMultiMethod("string",
		[](instance<PStringer> a) -> instance<std::string>
	{
		return instance<std::string>::make(a.getFeature<PStringer>()->toString(a));
	});
	semantics->builtin_implementMultiMethod("string",
		[](instance<PRepr> a) -> instance<std::string>
	{
		return instance<std::string>::make(a.getFeature<PRepr>()->toRepr(a));
	});
	semantics->builtin_implementMultiMethod("string",
		[](instance<> a) -> instance<std::string>
	{
		if (a.isNull()) return instance<std::string>::make("null");
		return instance<std::string>::make(a.toString());
	});

	semantics->builtin_implementMultiMethod("print",
		[](types::VarArgs<instance<>> args)
	{
		for (auto& it : args.args)
		{
			std::cout << *Execution::exec_fromCurrentModule("string", { it }).asType<std::string>();
			std::cout << " ";
		}
		std::cout << "\n";
	});

	semantics->builtin_implementMultiMethod("format",
		[](instance<std::string> fmt, types::VarArgs<instance<>> args) -> instance<std::string>
	{
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
					target = Execution::exec_fromCurrentModule(match, {});
				}

				instance<std::string> c = Execution::exec_fromCurrentModule("string", { target });
				s << *c;
			}
			else
			{
				s << m;
			}
		});

		return instance<std::string>::make(s.str());
	});

	semantics->builtin_implementMultiMethod("log",
		[](instance<Symbol> sym, instance<std::string> str, types::VarArgs<instance<>> args)
	{
		auto size = args.args.size() + 1;
		if (size != 1)
		{
			GenericInvoke invoke(size);
			invoke.args.push_back(str); std::copy(args.args.begin(), args.args.end(), std::back_inserter(invoke.args));
			str = Execution::exec_fromCurrentModule("format", invoke).asType<std::string>();
		}

		auto ns = Execution::getCurrent()->getNamespace();

		spdlog::level::level_enum level = spdlog::level::level_enum::off;
		if (sym->isKeyword())
		{
			auto critical = ns->symbolStore.intern("critical");
			if (critical == sym->at(1)) level = spdlog::level::level_enum::critical;
			auto error = ns->symbolStore.intern("error");
			if (error == sym->at(1)) level = spdlog::level::level_enum::err;
			auto warning = ns->symbolStore.intern("warning");
			if (warning == sym->at(1)) level = spdlog::level::level_enum::warn;
			auto info = ns->symbolStore.intern("info");
			if (info == sym->at(1)) level = spdlog::level::level_enum::info;
			auto debug = ns->symbolStore.intern("debug");
			if (debug == sym->at(1)) level = spdlog::level::level_enum::debug;
			auto trace = ns->symbolStore.intern("trace");
			if (trace == sym->at(1)) level = spdlog::level::level_enum::trace;
		}

		if (level == spdlog::level::level_enum::off)
			throw stdext::exception("Unknown level {0}", sym->getDisplay());

		ns->getEnvironment()->log()->log(level, str->c_str());
	});

	semantics->builtin_implementMultiMethod("log",
		[](instance<Symbol> sym, instance<> thing)
	{
		auto str = Execution::exec_fromCurrentModule("string", { thing }).asType<std::string>();
		Execution::exec_fromCurrentModule("format", { sym, str }).asType<std::string>();
	});

	semantics->builtin_implementMultiMethod("string/concat",
		[](instance<std::string> a, instance<std::string> b) -> instance<std::string>
	{
		std::string res = fmt::format("{0}{1}", *a, *b);
		return instance<std::string>::make(res);
	});

	semantics->builtin_implementMultiMethod("string/split",
		[](instance<std::string> a, instance<std::string> b) -> instance<List>
	{
		std::vector<std::string> sm;
		stdext::split(*a, *b, std::back_inserter(sm));

		std::vector<instance<>> res;
		for (auto i : sm)
		{
			res.push_back(instance<std::string>::make(i));
		}

		return instance<List>::make(res);
	});

	semantics->builtin_implementMultiMethod("string/join",
		[](instance<List> a, instance<std::string> b) -> instance<std::string>
	{
		std::cout << *Execution::exec_fromCurrentModule("string", { a }).asType<std::string>();

		std::ostringstream res;

		auto count = 0;
		for (auto& i : a->data())
		{
			std::cout << *Execution::exec_fromCurrentModule("string", { i }).asType<std::string>();
			if (count != (*a->size()) - 1) res << *b;
			count++;
		}

		return instance<std::string>::make(res.str());
	});

	semantics->builtin_implementMultiMethod("string/join",
		[](instance<List> a) -> instance<std::string>
	{
		return Execution::exec_fromCurrentModule("string/join", { a, instance<std::string>::make("\n") }).asType<std::string>();
	});


	semantics->builtin_implementMultiMethod("string/slice",
		[](instance<std::string> a, instance<int64_t> b, instance<int64_t> c) -> instance<std::string>
	{
		return instance<std::string>::make(a->substr(*b, *c));
	});

	semantics->builtin_implementMultiMethod("string/reverse",
		[](instance<std::string> a) -> instance<std::string>
	{
		auto res = instance<std::string>::make(*a);
		std::reverse(res->begin(), res->end());
		return res;
	});

	semantics->builtin_implementMultiMethod("string/includes",
		[](instance<std::string> a, instance<std::string> b) -> instance<bool>
	{
		return instance<bool>::make(a->find(*b) != a->npos);
	});

	semantics->builtin_implementMultiMethod("string/compare",
		[](instance<std::string> a, instance<std::string> b) -> instance<int64_t>
	{
		return instance<int64_t>::make(std::strcmp(a->c_str(), b->c_str()));
	});

	semantics->builtin_implementMultiMethod("string/isprefix",
		[](instance<std::string> a, instance<std::string> b) -> instance<bool>
	{
		bool res;
		if (a->size() > b->size()) res = false;
		else
		{
			res = *a == b->substr(0, a->size());
		}

		return instance<bool>::make(res);
	});

	semantics->builtin_implementMultiMethod("string/length",
		[](instance<std::string> a) -> instance<uint64_t>
	{
		return instance<uint64_t>::make(a->size());
	});
}
