
#include "lisp/common.h"
#include "lisp/library/libraries.h"


using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_http_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto fetch = instance<MultiMethod>::make();
	fetch->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, std::string>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		std::function<std::string(void*, size_t)> f = [](void* d, size_t s) {
			return std::string((char*)d, s);
		};

		craft::net::HTTPRequest opts;
		opts.type = craft::net::HTTPMethod::GET;
		opts.user_agent = "craftengine/test";


		stdext::future<std::string> fut = craft::net::fetch(
			*a,
			opts,
			f
		);

		std::string s = fut.get();

		return instance<std::string>::make(s);
	}));

	ret->define_eval("fetch", fetch);
}
