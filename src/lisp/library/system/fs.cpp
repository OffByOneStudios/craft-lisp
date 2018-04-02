
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

namespace _impl {
#ifdef _WIN32
	extern std::string GetLastErrorAsString();
#endif
}

void system::make_fs_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto ls = instance<MultiMethod>::make();
	ls->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<List>(),
		[](auto frame, auto args)
	{
		auto files = path::list_files(path::absolute());
		auto dirs = path::list_dirs(path::absolute());

		auto res = std::vector<instance<>>();
		for (auto f : files)
		{
			res.push_back(instance<std::string>::make(f));
		}

		for (auto f : dirs)
		{
			res.push_back(instance<std::string>::make(f));
		}

    return frame->getNamespace()->lookup("list")->getValue(frame).template asType<MultiMethod>()->call(frame, res);
		
	}));
	ret->define_eval("ls", ls);

	ls->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, List>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));

		auto files = path::list_files(*a);
		auto dirs = path::list_dirs(*a);

		auto res = std::vector<instance<>>();
		for (auto f : files)
		{
			res.push_back(instance<std::string>::make(f));
		}

		for (auto f : dirs)
		{
			res.push_back(instance<std::string>::make(f));
		}

    return frame->getNamespace()->lookup("list")->getValue(frame).template asType<MultiMethod>()->call(frame, res);
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
		outfile.write(b->c_str(), b->size());
		outfile.close();

		return instance<>();
	}));
	write->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<std::string, std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		instance<std::string> c(expect<std::string>(args[1]));
		instance<std::string> b(expect<std::string>(args[2]));

		std::ofstream::openmode op;
		if (*c == "b") op = std::ofstream::binary;
		else if (*c == "a") op = std::ofstream::app;
		else if (*c == "ba" || *c == "ba") op = std::ofstream::app | std::ofstream::binary;
		else throw stdext::exception("Unknown File Open Mode: {0}", *c);
		std::ofstream outfile;
		outfile.open(*a, op);
		outfile.write(b->c_str(), b->size());
		outfile.close();

		return instance<>();
	}));

	ret->define_eval("write", write);


	auto move = instance<MultiMethod>::make();
	move->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));


		auto f = path::normalize(*a);
		auto t = path::normalize(*b);
		if (rename(f.c_str(), t.c_str())) throw stdext::exception("{0}", "TODO Rename Failure");
		return instance<>();
	}));
	ret->define_eval("mv", move);


	auto glob = instance<MultiMethod>::make();
	glob->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string, List>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));
		auto res = std::vector<instance<>>();

		auto location = *a;
		auto glob = *b;

		auto tmp = path::filename(glob);
		tmp = std::regex_replace(tmp, std::regex("\\."), "\\.");
		auto filename = std::regex_replace(tmp, std::regex("\\*"), ".+");
		std::regex re(filename);

		
		std::vector<std::string> search_paths;

		search_paths.push_back(*a);
		path::walk w(search_paths[0]);
		for (auto it : w)
		{
			for (auto d : it.dirs)
			{
				search_paths.push_back(path::join(it.path, d));
			}
		}

		for (auto sp : search_paths)
		{
			for (auto f : path::list_files(sp))
			{
				auto fname = path::filename(f);
				if (std::regex_match(fname, re))
				{
					res.push_back(instance<std::string>::make(path::join(sp, f)));
				}
			}
		}

    return frame->getNamespace()->lookup("list")->getValue(frame).template asType<MultiMethod>()->call(frame, res);
	}));
	ret->define_eval("glob", glob);

}
