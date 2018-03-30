
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void system::make_string_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto concat = instance<MultiMethod>::make();
	concat->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));
		std::string res = fmt::format("{0}{1}", *a, *b);
		return instance<std::string>::make(res);
	}));
	ret->define_eval("cat", concat);

	auto split = instance<MultiMethod>::make();
	split->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));

		std::vector<std::string> sm;
		stdext::split(*a, *b, std::back_inserter(sm));

		std::vector<instance<>> res;
		for (auto i : sm)
		{
			res.push_back(instance<std::string>::make(i));
		}

		return frame->getNamespace()->lookup("list")->getValue(frame).asType<MultiMethod>()->call(frame, res);
	}));
	ret->define_eval("split", split);


	auto str = instance<MultiMethod>::make();
	str->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		return a;
	}));
	str->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<PStringer, std::string>(),
		[](auto frame, auto args)
	{
		instance<> a(args[0]);
		return instance<std::string>::make(a.getFeature<PStringer>()->toString(a));
	}));
	str->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<PIdentifier, std::string>(),
		[](auto frame, auto args)
	{
		instance<> a(args[0]);
		return instance<std::string>::make(a.getFeature<PIdentifier>()->identifier());
	}));
	ret->define_eval("str", str);

	auto _print = instance<MultiMethod>::make();
	_print->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeCollectArgs(),
		[str](instance<SFrame> frame, std::vector<instance<>> args) mutable
	{
		for (auto i : args) {
			std::cout << str->call(frame, { i }) << " ";
		}

		std::cout << "\n";
		
		return instance<>();
	}));
	ret->define_eval("print", _print);

	auto fmt = instance<MultiMethod>::make();
	fmt->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeCollectArgs(),
		[str](instance<SFrame> frame, std::vector<instance<>> args) mutable
	{
		std::regex re("\\{[^\\}]+\\}");
		std::ostringstream s;
		instance<std::string> a(expect<std::string>(args[0]));

		auto& tmp = *a;
		std::sregex_token_iterator
			begin(tmp.begin(), tmp.end(), re, { -1,0 }),
			end;

		std::for_each(begin, end, [&](std::string const& m) {

			if (std::regex_match(m, re))
			{
				std::string match = m.substr(1, m.size() - 2);
				instance<> target;
				
				
				ssize_t i = -1;
				try
				{
					i = std::stoi(match) + 1;
				}
				catch (...)
				{

				}
				if(i != -1)
				{
					if (size_t(i) >= args.size())
					{
						throw stdext::exception("Argument Reference {0} Out of Bounds", i - 1);
					}
					target = args[i];
				}
				else
				{
					target = frame->getNamespace()->lookup(match)->getValue(frame);
				}

				instance<std::string> c = str->call(frame, { target });
				s << *c;
			}
			else
			{
				s << m;
			}
		});
		return instance<std::string>::make(s.str());
	}));
	ret->define_eval("fmt", fmt);
}
