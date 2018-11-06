
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

Array::Array(types::TypeId t)
{ 
    _type = t;
    _typeSize = types::system().graph().getFirstPropValue<types::GraphPropertyCppSize>(t);
}

Array::Array(types::TypeId t, instance<uint64_t> s)
{ 
    _type = t;
    _typeSize = types::system().graph().getFirstPropValue<types::GraphPropertyCppSize>(t);
    _data.resize(*s);
}

Array::Array(types::TypeId t, std::vector<instance<>> const& data)
{ 
    _type = t;
    _typeSize = types::system().graph().getFirstPropValue<types::GraphPropertyCppSize>(t);
    for(auto i = 0; i < data.size(); ++i)
    {
        if(data[i].typeId() != t)
            throw stdext::exception("Elements of array must be of the same type");
    }
    
    _data.resize(data.size() * _typeSize);
    auto _dPtr = (uint8_t*)_data.data();
    for(auto i = 0; i < data.size(); ++i)
    {
        memcpy(_dPtr + (i * _typeSize), data[i].get(), _typeSize);
    }
}



instance<int64_t> Array::size()
{
	return instance<int64_t>::make(_data.size() / _typeSize);
}

instance<> Array::at(instance<int64_t> i)
{
	if (size_t(*i) < _data.size() / _typeSize)
    {
        auto res = _type.getFeature<PConstructor>()->construct();
        auto dP = _data.data() + size_t(*i) * _typeSize;
        memcpy(res.get(), (void*)dP, _typeSize);
        return res;
    } 
	throw stdext::exception("Array Index Out Of Bounds");
}

void Array::set(instance<int64_t> i, instance<> v) {
    if (size_t(*i) <= _data.size() / _typeSize)
    {
        auto start = (*i / _typeSize);
        auto dest = _data.data() + size_t(*i) * _typeSize;
        auto dPtr = (uint8_t*)v.get();
        memcpy(dest, (void*)dPtr, _typeSize);
    }
    throw stdext::exception("Array Index Out Of Bounds");
}

void Array::insert(instance<int64_t> i, instance<> v) {
    if (size_t(*i) <= _data.size() / _typeSize)
    {
        auto start = (*i / _typeSize);
        auto dPtr = (uint8_t*)v.get();
        for(uint8_t j = 0; j < _typeSize; ++j)
        {
            _data.insert(_data.begin() + start + j, std::byte(*(dPtr + j)));
        }
    }
    throw stdext::exception("Array Index Out Of Bounds");
}

void Array::erase(instance<int64_t> i) {
	//_data.erase(_data.begin() + *i);
}

void Array::push(instance<> v)
{
    auto dPtr = (uint8_t*)v.get();
    for(uint8_t j = 0; j < _typeSize; ++j)
    {
        _data.push_back(std::byte(*(dPtr + j)));
    }
}

void Array::pop()
{
	for(uint8_t j = 0; j < _typeSize; ++j)
    {
        _data.pop_back();
    }
}

instance<uint64_t> Array::dataSize()
{
    return instance<uint64_t>::make(_typeSize);
}

instance<uint64_t> Array::dataPtr()
{
    return instance<uint64_t>::make(uint64_t(_data.data()));
}

CRAFT_DEFINE(Array)
{
	//_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
	//	->withMethod("size", &List::size)
	//	->withMethod("slice", &List::slice)
	//	//->withMethod("at", &List::at)
	//	//->withMethod("insert", &List::insert)
	//	->withMethod("erase", &List::erase)
	//	->withMethod("reverse", &List::reverse)
	//	//->withMethod("push", &List::push)
	//	//->withMethod("pop", &List::pop)
	//	;

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<Array> l) -> std::string
	{
		std::ostringstream res;
		auto s = *l->size();
		res << "[";
		for (auto i = 0; i < s; i++)
		{
			auto it = l->at(instance<int64_t>::make(i));
			if (it.get() == nullptr) res << "null";
			else if (it.hasFeature<PRepr>()) res << it.getFeature<PRepr>()->toRepr(it);
			else
			{
				res << fmt::format("{0}<{1}>", it.typeId(), (void*)it.get());
			}
			if(i != s - 1) res << ", ";
		}
		res << "]";

		return res.str();

	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<Array> l) -> std::string
	{
		return l.getFeature<PStringer>()->toString(l);
	});
	_.defaults();
}


void core::make_array_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_defineType<Array>("Array");

	semantics->builtin_implementMultiMethod("array",
		[](instance<types::Graph::Node> t, types::VarArgs<instance<>> args)
	{
		return instance<Array>::make((types::TypeId)t.get(), args.args);
	});

	semantics->builtin_implementMultiMethod("array/size",
		[](instance<Array> a)
	{
		return a->size();
	});

	semantics->builtin_implementMultiMethod("array/get",
		[](instance<Array> a, instance<int64_t> b)
	{
		return a->at(b);
	});

	semantics->builtin_implementMultiMethod("array/set", 
		[](instance<Array> a, instance<int64_t> b, instance<> c)
	{
		a->set(b, c);
	});

	semantics->builtin_implementMultiMethod("array/insert", 
		[](instance<Array> a, instance<int64_t> b, instance<> c)
	{
		a->insert(b, c);
	});
	
	semantics->builtin_implementMultiMethod("array/erase",
		[](instance<Array> a, instance<int64_t> b)
	{
		a->erase(b);
	});

	semantics->builtin_implementMultiMethod("array/pop",
		[](instance<Array> a)
	{
		a->pop();
	});

	semantics->builtin_implementMultiMethod("array/push",
		[](instance<Array> a, instance<> b)
	{
		a->push(b);
	});
}
