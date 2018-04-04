#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp {
namespace library
{

	class Buffer
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Buffer);
	protected:
		std::vector<uint8_t> _data;

	public:
		CRAFT_LISP_EXPORTED Buffer(instance<int64_t> size);
		CRAFT_LISP_EXPORTED instance<int64_t> size();
		CRAFT_LISP_EXPORTED instance<int64_t> at(instance<int64_t> i);
		CRAFT_LISP_EXPORTED void set(instance<int64_t> i, instance<int64_t> d);
		CRAFT_LISP_EXPORTED std::vector<uint8_t>& data();
		CRAFT_LISP_EXPORTED instance<Buffer> join(instance<Buffer> other);
	};

	class List
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::List);
	protected:
		std::vector<instance<>> _data;

	public:

		CRAFT_LISP_EXPORTED instance<int64_t> size();
		CRAFT_LISP_EXPORTED instance<> at(instance<int64_t> i);
		CRAFT_LISP_EXPORTED void insert(instance<int64_t> i, instance<> v);
		CRAFT_LISP_EXPORTED void erase(instance<int64_t> i);
		CRAFT_LISP_EXPORTED void reverse();
		CRAFT_LISP_EXPORTED void push(instance<> i);
		CRAFT_LISP_EXPORTED instance<> pop();
		CRAFT_LISP_EXPORTED instance<List> slice(instance<int64_t> i, instance<int64_t> j);
		CRAFT_LISP_EXPORTED std::vector<instance<>>& data();
	};

	class Map
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Map);
	private:
		struct cmpByInstanceSemantics {
			inline bool operator()(const instance<>& a, const instance<>& b) const {
				if (a.typeId() != b.typeId()) return a.typeId() < b.typeId();

				if (a.typeId().isType<std::string>())
					return *a.asType<std::string>() < *b.asType<std::string>();
				else if (a.typeId().isType<int64_t>())
					return *a.asType<int64_t>() < *b.asType<int64_t>();
				else return a.get() < b.get();
			}
		};
	protected:
		std::map<instance<>, instance<>, cmpByInstanceSemantics> _data;

	public:

		CRAFT_LISP_EXPORTED instance<int64_t> size();

		CRAFT_LISP_EXPORTED instance<> at(instance<> i);

		CRAFT_LISP_EXPORTED void insert(instance<> i, instance<> v);
		CRAFT_LISP_EXPORTED void erase(instance<> i);
		CRAFT_LISP_EXPORTED std::map<instance<>, instance<>, cmpByInstanceSemantics>& data();
		CRAFT_LISP_EXPORTED std::vector<instance<>> keys();
		CRAFT_LISP_EXPORTED std::vector<instance<>> values();
	};

	class HttpServer
		: public craft::net::HttpServer
		, public virtual types::Object

	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::HttpServer);

	public:
		CRAFT_LISP_EXPORTED HttpServer(std::shared_ptr<spdlog::logger> logger, int port);

		inline virtual ~HttpServer() {};
	};

	class HttpEchoServer
		: public craft::net::HTTPRequestHandler
	{
		
	private:
	public:
		CRAFT_LISP_EXPORTED virtual bool handle(net::HTTPRequest& req, net::HttpResponse& rep) override;
	};
}}}