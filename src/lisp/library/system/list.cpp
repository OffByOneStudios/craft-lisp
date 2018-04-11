
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

List::List(std::vector<instance<>> && data)
	: _data(data)
{ }

instance<int64_t> List::size()
{
	return instance<int64_t>::make(_data.size());
}

instance<> List::at(instance<int64_t> i)
{
	if ((*i) < _data.size()) return _data[*i];
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

	if (ti > tj || ti < 0 || ti >= s || tj < 0) return res;

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
		->withMethod("at", &List::at)
		->withMethod("insert", &List::insert)
		->withMethod("erase", &List::erase)
		->withMethod("reverse", &List::reverse)
		->withMethod("push", &List::push)
		->withMethod("pop", &List::pop);

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
				res << fmt::format("{0}<{1}>", it.getFeature<PIdentifier>()->identifier(), (void*)it.get());
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

	auto list = instance<MultiMethod>::make();
	list->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		auto res = instance<List>::make();
		for (auto i : args)
		{
			res->push(i);
		}
		return res;
	}));
	ret->define_eval("list", list);

	semantics->builtin_implementMultiMethod("list/get",
		[](instance<List> a, instance<int64_t> b) -> instance<>
	{
		return a->at(b);
	});

	auto insert = instance<MultiMethod>::make();
	insert->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<int64_t> b(expect<int64_t>(args[1]));
		a->insert(b, args[2]);
		return instance<>();
	}));
	ret->define_eval("list/insert", insert);


	auto erase = instance<MultiMethod>::make();
	erase->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, int64_t>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<int64_t> b(expect<int64_t>(args[1]));

		a->erase(b);
		return instance<>();
	}));
	ret->define_eval("list/erase", erase);

	auto pop = instance<MultiMethod>::make();
	pop->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		return a->pop();
	}));
	ret->define_eval("list/pop", pop);

	auto push = instance<MultiMethod>::make();
	push->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		a->push(args[1]);
		return instance<>();
	}));
	ret->define_eval("list/push", push);

	auto slice = instance<MultiMethod>::make();
	slice->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, int64_t, int64_t>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<int64_t> b(expect<int64_t>(args[1]));
		instance<int64_t> c(expect<int64_t>(args[2]));
		return a->slice(b, c);
	}));
	ret->define_eval("list/slice", slice);

	auto reverse = instance<MultiMethod>::make();
	reverse->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		a->reverse();
		return instance<>();
	}));
	ret->define_eval("list/reverse", reverse);


	auto fmap = instance<MultiMethod>::make();
	fmap->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, Function>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<Function> b(expect<Function>(args[1]));

		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto call = instance<Sexpr>::make();
			call->cells = { b, i, count };
			res->push(frame->getNamespace()->environment()->eval(frame, call));
			(*count)++;
		}

		return res;
	}));
	fmap->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, Closure>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<Closure> b(expect<Closure>(args[1]));

		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto call = instance<Sexpr>::make();
			call->cells = { b, i, count };
			res->push(frame->getNamespace()->environment()->eval(frame, call));
			(*count)++;
		}
		return res;
	}));
	ret->define_eval("list/fmap", fmap);

	auto filter = instance<MultiMethod>::make();
	filter->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, Function>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<Function> b(expect<Function>(args[1]));

		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto call = instance<Sexpr>::make();
			call->cells = { b, i, count };
			auto isIn = frame->getNamespace()->environment()->eval(frame, call);
			if (isIn.typeId().isType<bool>())
			{
				bool f = *isIn.asType<bool>();
				if (f) res->push(i);
			} 
			else if (isIn)  res->push(i);
			(*count)++;
		}

		return res;
	}));
	filter->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, Closure>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<Closure> b(expect<Closure>(args[1]));

		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto call = instance<Sexpr>::make();
			call->cells = { b, i, count };
			auto isIn = frame->getNamespace()->environment()->eval(frame, call);
			if (isIn.typeId().isType<bool>())
			{
				bool f = *isIn.asType<bool>();
				if (f) res->push(i);
			}
			else if (isIn)  res->push(i);
			(*count)++;
		}

		return res;
	}));
	ret->define_eval("list/filter", filter);
}
