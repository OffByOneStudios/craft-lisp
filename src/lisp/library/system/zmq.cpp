#include "lisp/common.h"

#include "zmq/zmq.h"

#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

#include "zmq.h"




using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


CRAFT_OBJECT_DEFINE(ZMessage)
{
	_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("get", &ZMessage::get)
		->withMethod("fill", &ZMessage::fill);
	_.defaults();
}

CRAFT_OBJECT_DEFINE(ZSocket)
{
	_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("send", &ZSocket::send)
		->withMethod("recv", &ZSocket::recv);
	_.defaults();
}

CRAFT_OBJECT_DEFINE(ZContext)
{
	_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("bind", &ZContext::bind)
		->withMethod("connect", &ZContext::connect);
	_.defaults();
}


ZMessage::ZMessage()
{
	_msg = new zmq_msg_t;
	memset(_msg, 0, sizeof(zmq_msg_t));
}

ZMessage::~ZMessage()
{
	zmq_msg_t* p = (zmq_msg_t*)_msg;
	zmq_msg_close(p);
	delete _msg;
}

instance<std::string> ZMessage::get()
{
	zmq_msg_t* p = (zmq_msg_t*)_msg;
	size_t s = zmq_msg_size(p);
	void* d = zmq_msg_data(p);

	auto res = instance<std::string>::make();
	res->resize(s);
	
	std::memcpy(const_cast<char*>(res->data()), d, s);
	return res;
}


void ZMessage::fill(instance<std::string> s)
{
	_owned = s;
	zmq_msg_t* p = (zmq_msg_t*)_msg;
	zmq_msg_init_data(p, const_cast<char*>(s->data()), s->size() + 1, nullptr, nullptr);
}


ZSocket::ZSocket()
{
	_socket = nullptr;
}

ZSocket::~ZSocket()
{
	if (_socket)
	{
		zmq_close(_socket);
	}
}

void ZSocket::send(instance<ZMessage> s, instance<bool> more)
{
	auto flags = (*more) ? ZMQ_SNDMORE : 0;
	auto errval = zmq_sendmsg(_socket, (zmq_msg_t*)s->_msg, flags);
	if (errval == -1) throw stdext::exception("{0}", zmq_strerror(zmq_errno()));
}

instance<ZMessage> ZSocket::recv()
{
	auto res = instance<ZMessage>::make();
	zmq_msg_init((zmq_msg_t*)res->_msg);

	auto errval = zmq_msg_recv((zmq_msg_t*)res->_msg, _socket, 0);
	
	if (errval == -1) throw stdext::exception("{0}", zmq_strerror(zmq_errno()));

	return res;
}

instance<bool> craft::lisp::ZSocket::recv_more()
{
	int more;
	size_t more_size = sizeof(more);

	auto rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &more, &more_size);
	return instance<bool>::make((more != 0));
}


ZContext::ZContext()
{
	_ctx = zmq_ctx_new();
}


ZContext::~ZContext()
{
	zmq_ctx_destroy(_ctx);
}

instance<ZSocket> ZContext::bind(instance<std::string> kind, instance<int64_t> port)
{
	int zmq_type;
	if ((*kind) == "PUB")
	{
		zmq_type = ZMQ_PUB;
	}
	else if ((*kind) == "PUSH")
	{
		zmq_type = ZMQ_PUSH;
	}
	else if ((*kind) == "REP")
	{
		zmq_type = ZMQ_REP;
	}
	else if ((*kind) == "STREAM")
	{
		zmq_type = ZMQ_STREAM;
	}
	else
	{
		throw stdext::exception("Unsupported Bindable Kind {0}", *kind);
	}
	auto res = instance<ZSocket>::make();

	res->_socket = zmq_socket(_ctx, zmq_type);
	auto uri = fmt::format("tcp://*:{0}", *port.get());
	auto err = zmq_bind(res->_socket, uri.c_str());

	if (err == -1) throw stdext::exception("{0}", zmq_strerror(zmq_errno()));

	return res;
}

instance<ZSocket> ZContext::connect(instance<std::string> kind, instance<std::string> uri)
{
	int zmq_type;
	if ((*kind) == "SUB")
	{
		zmq_type = ZMQ_PUB;
	}
	else if ((*kind) == "PULL")
	{
		zmq_type = ZMQ_PUSH;
	}
	else if ((*kind) == "REQ")
	{
		zmq_type = ZMQ_REQ;
	}
	else
	{
		throw stdext::exception("Unsupported Bindable Kind {0}", *kind);
	}
	auto res = instance<ZSocket>::make();

	res->_socket = zmq_socket(_ctx, zmq_type);
	auto err = zmq_connect(res->_socket, uri->c_str());

	if (err == -1) throw stdext::exception("{0}", zmq_strerror(zmq_errno()));

	return res;
}

void lisp::library::system::make_zmq_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto zcontext = instance<MultiMethod>::make();
	zcontext->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		return instance<ZContext>::make();
	}));
	ret->define_eval("zcontext", zcontext);


	auto zbind = instance<MultiMethod>::make();
	zbind->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<ZContext, std::string, int64_t, ZSocket>(),
		[](auto frame, auto args)
	{
		instance<ZContext> a(expect<ZContext>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));
		instance<int64_t> c(expect<int64_t>(args[2]));

		return a->bind(b, c);
	}));
	ret->define_eval("zbind", zbind);

	auto zconnect = instance<MultiMethod>::make();
	zconnect->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<ZContext, std::string, std::string, ZSocket>(),
		[](auto frame, auto args)
	{
		instance<ZContext> a(expect<ZContext>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));
		instance<std::string> c(expect<std::string>(args[2]));

		return a->connect(b, c);
	}));
	ret->define_eval("zconnect", zconnect);

	auto zmsg = instance<MultiMethod>::make();
	zmsg->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, ZMessage>(),
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));

		auto res = instance<ZMessage>::make();
		res->fill(a);
		return res;
	}));
	zmsg->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<ZMessage, std::string>(),
		[](auto frame, auto args)
	{
		instance<ZMessage> a(expect<ZMessage>(args[0]));
		return a->get();
	}));
	ret->define_eval("zmsg", zmsg);

	auto zsend = instance<MultiMethod>::make();
	zsend->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<ZSocket, ZMessage>(),
		[](auto frame, auto args)
	{
		instance<ZSocket> a(expect<ZSocket>(args[0]));
		instance<ZMessage> b(expect<ZMessage>(args[1]));
		a->send(b, instance<bool>::make(false));
		return instance<>();
	}));
	zsend->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<ZSocket, ZMessage, bool>(),
		[](auto frame, auto args)
	{
		instance<ZSocket> a(expect<ZSocket>(args[0]));
		instance<ZMessage> b(expect<ZMessage>(args[1]));
		instance<bool> c(expect<bool>(args[2]));
		a->send(b, c);
		return instance<>();
	}));
	ret->define_eval("zsend", zsend);

	auto zrecv = instance<MultiMethod>::make();
	zrecv->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<ZSocket, ZMessage>(),
		[](auto frame, auto args)
	{
		instance<ZSocket> a(expect<ZSocket>(args[0]));
		return a->recv();
	}));
	ret->define_eval("zrecv", zrecv);

	auto zmore = instance<MultiMethod>::make();
	zmore->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<ZSocket>(),
		[](auto frame, auto args)
	{
		instance<ZSocket> a(expect<ZSocket>(args[0]));
		return a->recv_more();
	}));
	ret->define_eval("zmore", zmore);
}