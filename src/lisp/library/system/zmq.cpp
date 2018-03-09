#include "lisp/common.h"

#include "../libraries.h"
#include "./zmq.h"

#include "zmq/zmq.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;


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

void ZSocket::send(instance<ZMessage> s)
{
	auto errval = zmq_sendmsg(_socket, (zmq_msg_t*)s->_msg, 0);
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

void lisp::library::system::make_zmq_globals(instance<Module>& m, instance<Namespace>& ns)
{

}