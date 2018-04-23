
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

List::List(std::vector<instance<>> const& data)
	: _data(data)
{ }

instance<int64_t> List::size()
{
	return instance<int64_t>::make(_data.size());
}

instance<> List::at(instance<int64_t> i)
{
	if (size_t(*i) < _data.size()) return _data[size_t(*i)];
	throw stdext::exception("List Index Out Of Bounds");
}

void List::insert(instance<int64_t> i, instance<> v) {
	_data.insert(_data.begin() + *i, v);
}

void List::erase(instance<int64_t> i) {
	_data.erase(_data.begin() + *i);
}

void List::reverse() {
	std::reverse(_data.begin(), _data.end());
}

void List::push(instance<> i)
{
	_data.push_back(i);
}

instance<> List::pop()
{
	if (!_data.size()) return instance<>();

	auto res = _data.back();
	_data.pop_back();
	return res;
}

std::vector<instance<>>& List::data()
{
	return _data;
}

instance<List> List::slice(instance<int64_t> i, instance<int64_t> j)
{
	auto res = instance<List>::make();
	auto ti = *i; auto tj = std::min(int64_t(_data.size()), *j);
	auto s = _data.size();

	if (ti > tj || ti < int64_t(0) || ti >= int64_t(s) || tj < int64_t(0)) return res;

	for (auto it = ti; it < tj; it++)
	{
		res->_data.push_back(_data[it]);
	}

	return res;
}


CRAFT_DEFINE(List)
{
	_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("size", &List::size)
		->withMethod("slice", &List::slice)
		//->withMethod("at", &List::at)
		//->withMethod("insert", &List::insert)
		->withMethod("erase", &List::erase)
		->withMethod("reverse", &List::reverse)
		//->withMethod("push", &List::push)
		//->withMethod("pop", &List::pop)
		;

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<List> l) -> std::string
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
				res << fmt::format("{0}<{1}>", it.typeId().toString(), (void*)it.get());
			}
			if(i != s - 1) res << ", ";
		}
		res << "]";

		return res.str();

	});

	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<List> l) -> std::string
	{
		return l.getFeature<PStringer>()->toString(l);
	});
	_.defaults();
}


void core::make_list_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("list",
		[](types::VarArgs<instance<>> args) -> instance<List>
	{
		return instance<List>::make(args.args);
	});

	semantics->builtin_implementMultiMethod("list/get",
		[](instance<List> a, instance<int64_t> b) -> instance<>
	{
		return a->at(b);
	});

	semantics->builtin_implementMultiMethod("list/insert", 
		[](instance<List> a, instance<int64_t> b, instance<> c)
	{
		a->insert(b, c);
	});
	
	semantics->builtin_implementMultiMethod("list/erase",
		[](instance<List> a, instance<int64_t> b)
	{
		a->erase(b);
	});

	semantics->builtin_implementMultiMethod("list/pop",
		[](instance<List> a)
	{
		a->pop();
	});

	semantics->builtin_implementMultiMethod("list/push",
		[](instance<List> a, instance<> b)
	{
		a->push(b);
	});

	semantics->builtin_implementMultiMethod("list/slice",
		[](instance<List> a, instance<int64_t> b, instance<int64_t> c) -> instance<List>
	{
		return a->slice(b, c);
	});

	semantics->builtin_implementMultiMethod("list/reverse",
		[](instance<List> a)
	{
		a->reverse();
	});

	semantics->builtin_implementMultiMethod("list/fmap",
		[](instance<List> a, instance<Function> b) -> instance<List>
	{
		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			Execution::exec(b, { i, count });
			(*count)++;
		}

		return res;
	});

	semantics->builtin_implementMultiMethod("list/fmap",
		[](instance<List> a, instance<Function> b) -> instance<List>
	{
		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			Execution::exec(b, { i, count });
			(*count)++;
		}

		return res;
	});

	semantics->builtin_implementMultiMethod("list/fmap",
		[](instance<List> a, instance<Function> b) -> instance<List>
	{
		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto isIn = Execution::exec(b, { i, count });
			if (isIn.typeId().isType<bool>())
			{
				bool f = *isIn.asType<bool>();
				if (f) res->push(i);
			}
			else if (isIn)  res->push(i);
			(*count)++;
		}

		return res;
	});
}
