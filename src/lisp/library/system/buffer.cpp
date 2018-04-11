
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
	_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("size", &Buffer::size)
		->withMethod("at", &Buffer::at)
		->withMethod("set", &Buffer::set)
		->withMethod("join", &Buffer::join);

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<Buffer> l) -> std::string
	{
		std::ostringstream res;

		

		res << "0x";
		for (auto it : l->data())
		{
			res << std::hex << it;

		}
		return res.str();

	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<Buffer> l) -> std::string
	{
		return l.getFeature<PStringer>()->toString(l);
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
	if (*i > _data.size()) throw stdext::exception("Index Out Of Range");

	return instance<int64_t>::make(_data[*i]);
}

void Buffer::set(instance<int64_t> i, instance<int64_t> j)
{
	if (*i > _data.size()) throw stdext::exception("Index Out Of Range");

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
}


