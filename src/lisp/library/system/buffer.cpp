
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

CRAFT_DEFINE(Buffer)
{
	/*_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("size", &Buffer::size)
		->withMethod("at", &Buffer::at)
		->withMethod("set", &Buffer::set)
		->withMethod("join", &Buffer::join);*/

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<Buffer> l) -> std::string
	{
		std::ostringstream res;

		for (auto it : l->data())
		{
			res << it;

		}
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<Buffer> l) -> std::string
	{
		std::ostringstream res;  res << "0x";
		for (auto it : l->data())
		{
			res << std::hex << it;

		}
		return res.str();
	});
	_.defaults();
}

Buffer::Buffer(instance<int64_t> size)
{
	_data.resize(*size);
}

instance<int64_t> Buffer::size()
{
	return instance<int64_t>::make(_data.size());
}

instance<int64_t> Buffer::at(instance<int64_t> i)
{
	if ((size_t)*i > _data.size()) throw stdext::exception("Index Out Of Range");

	return instance<int64_t>::make(_data[*i]);
}

void Buffer::set(instance<int64_t> i, instance<int64_t> j)
{
	if ((size_t)*i > _data.size()) throw stdext::exception("Index Out Of Range");

	_data[*i] = uint8_t(*j);

	return;
}

std::vector<uint8_t>& Buffer::data()
{
	return _data;
}

instance<Buffer> Buffer::join(instance<Buffer> other)
{
	auto size = _data.size() + other->_data.size();
	auto res = instance<Buffer>::make(instance<int64_t>::make(size));

	memcpy(res->_data.data(), _data.data(), _data.size());
	memcpy(res->_data.data() + _data.size() + 1, other->_data.data(), other->_data.size());

	return res;
}

void core::make_buffer_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();


	semantics->builtin_implementMultiMethod("join",
		[](types::VarArgs<instance<Buffer>> args) -> instance<Buffer>
	{
		size_t total = 0;
		for (auto& i : args.args)
		{
			total += *i->size();
		}

		auto res = instance<Buffer>::make(instance<int64_t>::make(total));
		
		auto& r_data = res->data();
		size_t p = 0;
		for (auto& i : args.args)
		{
			memcpy((uint8_t*)r_data.data() + p, i->data().data(), i->data().size());
			p += i->data().size();
		}

		return res;
	});

	semantics->builtin_implementMultiMethod("buffer/join",
		[](instance<List> buffer) -> instance<Buffer>
	{
		size_t total = 0;
		GenericInvoke res(*buffer->size());
		for (auto& i : buffer->data())
		{
			if (!i.isType<Buffer>()) throw stdext::exception("List must contain homogenous collection of Buffers");
			res.args.push_back(i.asType<Buffer>());
		}

		return Execution::exec_fromCurrentModule("join", res);
	});
}


