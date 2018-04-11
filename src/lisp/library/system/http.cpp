
#include "lisp/common.h"
#include "lisp/library/libraries.h"

#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


CRAFT_DEFINE(craft::lisp::library::HttpServer)
{
	_.defaults();
}

namespace craft {
namespace lisp {
namespace library {


	instance<std::string> stringMethod(net::HTTPMethod i) {
		switch (i)
		{
		case craft::net::HTTPMethod::GET:
			return instance<std::string>::make("GET");
		case craft::net::HTTPMethod::POST:
			return instance<std::string>::make("POST");
		case craft::net::HTTPMethod::PUT:
			return instance<std::string>::make("PUT");
		case craft::net::HTTPMethod::DELET:
			return instance<std::string>::make("DELETE");
		default:
			return instance<std::string>::make("GET");
		}
	}


	HttpServer::HttpServer(std::shared_ptr<spdlog::logger> logger, int port)
		: craft::net::HttpServer(logger, port)
	{

	}

	class HttpLispServer
		: public craft::net::HTTPRequestHandler
	{
		instance<Environment> _env;
		instance<Function> _handler;
	public:
		HttpLispServer(instance<Function> handler) {_handler = handler; }
	public:
		virtual bool handle(net::HTTPRequest& req, net::HttpResponse& rep) {
			rep.code = 200;
			rep.content_type = "text/html";

			// Gather Request
			auto mreq = instance<Map>::make();

			mreq->insert(instance<std::string>::make("path"), instance<std::string>::make(req.path));
			mreq->insert(instance<std::string>::make("method"), stringMethod(req.type));

			auto headers = instance<Map>::make();
			for (auto it : req.headers) 
			{
				headers->insert(instance<std::string>::make(it.first), instance<std::string>::make(it.second));
			}

			mreq->insert(instance<std::string>::make("headers"), headers);
			mreq->insert(instance<std::string>::make("size"), instance<int64_t>::make(req.body_size));
			mreq->insert(instance<std::string>::make("body"), instance<std::string>::make((char*)req.body, req.body_size));

			// Default Response
			auto mrep = instance<Map>::make();
			mrep->insert(instance<std::string>::make("code"), instance<int64_t>::make(200));
			mrep->insert(instance<std::string>::make("content-type"), instance<std::string>::make("text/plain"));
			
			// Invoke
			try
			{
				auto res = Execution::exec(_handler, { mreq, mrep });
				// Gather Response
				rep.content_type = *mrep->at(instance<std::string>::make("content-type")).asType<std::string>();
				rep.code = int16_t(*mrep->at(instance<std::string>::make("code")).asType<int64_t>());

				if (res.get() == nullptr) rep.data << "null";
				else if (res.hasFeature<PStringer>()) rep.data << res.getFeature<PStringer>()->toString(res);
				else if (res.hasFeature<PRepr>()) rep.data << res.getFeature<PRepr>()->toRepr(res);
				else rep.data << fmt::format("{0}<{1}>", res.getFeature<PIdentifier>()->identifier(), (void*)res.get());
			}
			catch (stdext::exception e)
			{
				rep.code = 500;
				rep.data << fmt::format("<H1> Server Fault</H1><p>{0}</p>", e.what());
			}
			
			return true;
		}
	};
}}}

void core::make_http_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("http/fetch",
		[](instance<std::string> a) -> instance<std::string>
	{
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
	});

	semantics->builtin_implementMultiMethod("http/server",
		[](instance<int64_t> a, instance<Function> b) -> instance<HttpServer>
	{
		auto server = instance<HttpServer>::make(spdlog::stdout_color_mt("http"), int32_t(*a));
		auto hands = new HttpLispServer(b);
		server->handlers.push_back(hands);
		server->init();

		return server;
	});

	semantics->builtin_implementMultiMethod("http/server/forever",
		[](instance<HttpServer> a)
	{
		a->serve_forever();
	});

	
}
