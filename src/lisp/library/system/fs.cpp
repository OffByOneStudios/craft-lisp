
#include "lisp/common.h"
#include "lisp/library/libraries.h"


using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void system::make_fs_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto ls = instance<MultiMethod>::make();
	ls->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string>(),
		[](auto frame, auto args)
	{
		auto files = path::list_files(path::absolute());
		auto dirs = path::list_dirs(path::absolute());

		return instance<std::string>::make(fmt::format("{0}\t{1}",
			stdext::join('\t', files.begin(), files.end()),
			stdext::join('\t', dirs.begin(), dirs.end()))
		);
	}));
	ret->define_eval("ls", ls);

	ls->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));

		auto files = path::list_files(*a);
		auto dirs = path::list_dirs(*a);

		return instance<std::string>::make(fmt::format("{0}\n{1}",
			stdext::join('\n', files.begin(), files.end()),
			stdext::join('\n', dirs.begin(), dirs.end()))
		);
	}));
	ret->define_eval("ls", ls);

	auto cwd = instance<MultiMethod>::make();
	cwd->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		return instance<std::string>::make(path::absolute());
	}));
	ret->define_eval("cwd", cwd);

	auto cd = instance<MultiMethod>::make();
	cd->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));

		path::set_cwd(*a);

		return instance<>();
	}));
	ret->define_eval("cd", cd);

	auto read = instance<MultiMethod>::make();
	read->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		auto text = craft::fs::read<std::string>(*a, &craft::fs::string_read).get();

		return instance<std::string>::make(text);
	}));
	ret->define_eval("read", read);

	auto write = instance<MultiMethod>::make();
	write->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));
		
		std::ofstream outfile;
		outfile.open(*a);
		outfile << b.get()->c_str();
		outfile.close();

		return instance<>();
	}));
	ret->define_eval("write", write);
}
