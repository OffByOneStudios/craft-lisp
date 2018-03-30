
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


namespace _impl {

	instance<std::string> getEnv(instance<std::string> s)
	{
#ifdef _WIN32
		std::string res(32767, '\0');
		auto rsize = GetEnvironmentVariable(s->c_str(), (LPSTR)res.data(), 32767);
		res.resize(rsize);
		return instance<std::string>::make(res);
#else
		char* pPath;
		pPath = getenv(a->c_str());
		return instance<std::string>::make(std::string(pPath));
#endif
	}

	instance<std::string> setEnv(instance<std::string> k, instance<std::string> v)
	{
#ifdef _WIN32
		std::string res(32767, '\0');
		auto rsize = GetEnvironmentVariable(k->c_str(), (LPSTR)res.data(), 32767);
		res.resize(rsize);

		SetEnvironmentVariable(k->c_str(), v->c_str());
		return instance<std::string>::make(res);
#else
		char* pPath;
		pPath = getenv(a->c_str());

		setenv(k->c_str(), v->c_str(), 1);

		return instance<std::string>::make(std::string(pPath));
#endif
	}
}


void system::make_env_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto _getenv = instance<MultiMethod>::make();
	_getenv->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		return _impl::getEnv(a);
	}));
	ret->define_eval("getenv", _getenv);

	auto _setenv = instance<MultiMethod>::make();
	_setenv->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));
		return _impl::setEnv(a, b);
	}));
	ret->define_eval("setenv", _setenv);

	
}
