#pragma once
#include "lisp/common.h"

//#include "lisp/Sexpr.h"
//CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, mpz_class);
//CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, mpq_class);
//CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, mpf_class);
namespace craft {
namespace lisp 
{

	class ZMessage
		: public virtual types::Object

	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::ZMessage)

	private:
		void* _msg;
		instance<std::string> _owned;
	public:
		CRAFT_LISP_EXPORTED ZMessage();
		CRAFT_LISP_EXPORTED ~ZMessage();


		CRAFT_LISP_EXPORTED instance<std::string> get();
		CRAFT_LISP_EXPORTED void fill(instance<std::string> s);

		friend class ZSocket;
	};

	class ZSocket
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::ZSocket)
	private:
		void* _socket;

	public:
		CRAFT_LISP_EXPORTED ZSocket();
		~ZSocket();

		CRAFT_LISP_EXPORTED void send(instance<ZMessage> s, instance<bool> more);
		CRAFT_LISP_EXPORTED instance<ZMessage> recv();
		CRAFT_LISP_EXPORTED instance<bool> recv_more();

		friend class ZContext;
	};


	class ZContext
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::ZContext)
	private:
		void* _ctx;

	public:
		CRAFT_LISP_EXPORTED ZContext();
		~ZContext();


		CRAFT_LISP_EXPORTED instance<ZSocket> bind(instance<std::string> kind, instance<int64_t> port);
		CRAFT_LISP_EXPORTED instance<ZSocket> connect(instance<std::string> kind, instance<std::string> uri);
	};
}}