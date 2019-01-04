
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

#ifdef _WIN32
#include <direct.h>  
#endif 
using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

#ifdef __GNUC__
#include <experimental/filesystem>
namespace sfs = std::experimental::filesystem;
#else
#include <filesystem>
namespace sfs = std::filesystem;
#endif




namespace _impl {
#ifdef _WIN32
	extern std::string GetLastErrorAsString();
#endif
}

void core::make_fs_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("ls",
		[]() -> instance<List>
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

		return instance<List>::make(res);
	});

	semantics->builtin_implementMultiMethod("ls",
		[](instance<std::string> a) -> instance<List>
	{
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

		return instance<List>::make(res);
	});

	semantics->builtin_implementMultiMethod("cwd",
		[]() -> instance<std::string>
	{
		return instance<std::string>::make(path::absolute());
	});

	semantics->builtin_implementMultiMethod("cd",
		[](instance<std::string> a)
	{
		path::set_cwd(*a);
	});

	semantics->builtin_implementMultiMethod("fs/mkdir", [](instance<std::string> s) {
		sfs::create_directories(*s);
	});

	semantics->builtin_implementMultiMethod("fs/read",
		[](instance<std::string> _0) -> instance<std::string>
	{
		auto text = craft::fs::read<std::string>(*_0, &craft::fs::string_read).get();

		return instance<std::string>::make(text);
	});

	semantics->builtin_implementMultiMethod("fs/write",
		[](instance<std::string> _0, instance<std::string> _1)
	{
		auto res = std::regex_replace(*_1, std::regex("\r\n"), "\n");
		std::ofstream outfile;
		outfile.open(*_0);
		outfile.write(res.c_str(), res.size());
		outfile.close();
	});
	semantics->builtin_implementMultiMethod("fs/write",
		[](instance<std::string> _0, instance<std::string> _1, instance<std::string> _2)
	{
		std::ofstream::openmode op;
		if (*_2 == "b") op = std::ofstream::binary;
		else if (*_2 == "a") op = std::ofstream::app;
		else if (*_2 == "ba" || *_2 == "ba") op = std::ofstream::app | std::ofstream::binary;
		else throw stdext::exception("Unknown File Open Mode: {0}", *_2);
		
		std::ofstream outfile;
		outfile.open(*_0, op);
		outfile.write(_1->c_str(), _1->size());
		outfile.close();
	});


	semantics->builtin_implementMultiMethod("fs/mv",
		[](instance<std::string> a, instance<std::string> b)
	{
		auto f = path::normalize(*a);
		auto t = path::normalize(*b);
		if (rename(f.c_str(), t.c_str())) throw stdext::exception("{0}", "TODO Rename Failure");
	});

	semantics->builtin_implementMultiMethod("fs/rm", [](instance<std::string> a) { remove(a->c_str());});
	semantics->builtin_implementMultiMethod("fs/exists", [](instance<std::string> a) { return instance<bool>::make(access(a->c_str(), 00) == 0); });
	semantics->builtin_implementMultiMethod("glob",
		[](instance<std::string> a, instance<std::string> b) -> instance<List>
	{
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

		return instance<List>::make(res);
	});
}
