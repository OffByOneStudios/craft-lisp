
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

#include <locale> 
#include <codecvt>

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

#ifndef _WIN32
  extern char** environ;
#endif

namespace _impl {
	instance<std::string> getEnv(instance<std::string> s)
	{
#ifdef _WIN32
		std::string res(32767, '\0');
		auto rsize = GetEnvironmentVariableA(s->c_str(), (LPSTR)res.data(), 32767);
		res.resize(rsize);
		return instance<std::string>::make(res);
#else
		char* pPath;
		pPath = getenv(s->c_str());
		return instance<std::string>::make(std::string(pPath));
#endif
	}

	instance<std::string> setEnv(instance<std::string> k, instance<std::string> v)
	{
#ifdef _WIN32
		std::string res(32767, '\0');
		auto rsize = GetEnvironmentVariableA(k->c_str(), (LPSTR)res.data(), 32767);
		res.resize(rsize);

		SetEnvironmentVariableA(k->c_str(), v->c_str());
		return instance<std::string>::make(res);
#else
		char* pPath;
		pPath = getenv(k->c_str());

		setenv(k->c_str(), v->c_str(), 1);

		return instance<std::string>::make(std::string(pPath));
#endif
	}
	instance<Map> listEnv()
	{
#ifdef _WIN32
		wchar_t* strings = (wchar_t*)GetEnvironmentStrings();
		wchar_t* b = strings;
#else
		char* b = (char*) environ;
#endif
		auto res = instance<Map>::make();

		bool found = false;
		while (*b)
		{
			auto kv = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(b);//std::string(b);
			b += kv.size() + 1;
			auto i = kv.find('=');
			res->insert(instance<std::string>::make(kv.substr(0, i)), instance<std::string>::make(kv.substr(i + 1)));
		}
			
#ifdef _WIN32
		FreeEnvironmentStringsW((LPWCH)strings);
#endif
		return res;
	}
}


void core::make_env_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("getenv",
		[](instance<std::string> a) -> instance<std::string>
	{
		return _impl::getEnv(a);
	});

	semantics->builtin_implementMultiMethod("setenv",
		[](instance<std::string> a, instance<std::string> b) -> instance<std::string>
	{
		return _impl::setEnv(a, b);
	
	semantics->builtin_implementMultiMethod("listenv",
		[]() -> Map
	{
		return _impl::listEnv();
	}));
}
