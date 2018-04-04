
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


CRAFT_OBJECT_DEFINE(craft::lisp::library::HttpServer)
{
	_.defaults();
}

namespace craft {
namespace lisp {
namespace library {

	HttpServer::HttpServer(std::shared_ptr<spdlog::logger> logger, int port)
		: craft::net::HttpServer(logger, port)
	{

	}

	bool HttpEchoServer::handle(net::HTTPRequest& req, net::HttpResponse& rep)
	{
		req.path = req.path.substr(1);

		rep.code = 200;
		rep.content_type = "text/html";
		rep.data << "<h1>Hello World</h1>";

		return true;
	}
}}}

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

	
	

	auto httpserver = instance<MultiMethod>::make();
	httpserver->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<int64_t, HttpServer>(),
		[&env](auto frame, auto args)
	{
		instance<int64_t> a(expect<int64_t>(args[0]));
		auto server = instance<HttpServer>::make(spdlog::stdout_color_mt("http"), int32_t(*a));
		auto hands = new HttpEchoServer();
		server->handlers.push_back(hands);
		server->init();

		return server;
	}));

	ret->define_eval("httpserver", httpserver);
}
