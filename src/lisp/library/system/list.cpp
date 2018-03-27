
#include "lisp/common.h"
#include "lisp/library/libraries.h"


using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


namespace craft {
namespace lisp {
namespace library {
	class List
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::List);
	private:
		std::vector<instance<>> _data;

	public:

		instance<int64_t> size()
		{
			return instance<int64_t>::make(_data.size());
		}

		instance<> at(instance<int64_t> i)
		{
			if ((*i) < _data.size()) return _data[*i];
			throw stdext::exception("List Index Out Of Bounds");
		}

		void insert(instance<int64_t> i, instance<> v) {
			_data.insert(_data.begin()+ *i, v);
		}

		void erase(instance<int64_t> i) {
			_data.erase(_data.begin() + *i);
		}

		void reverse() {
			std::reverse(_data.begin(), _data.end());
		}

		void push(instance<> i)
		{
			_data.push_back(i);
		}

		instance<> pop()
		{
			if (!_data.size()) return instance<>();

			auto res = _data.back();
			_data.pop_back();
			return res;
		}

		instance<List> slice(instance<int64_t> i, instance<int64_t> j)
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
	};
}}}

CRAFT_OBJECT_DEFINE(List)
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

		res << "[";
		for (auto i = 0; i < std::min(int64_t(10), *l->size()); i++)
		{
			auto it = l->at(instance<int64_t>::make(i));
			if (it.hasFeature<PStringer>()) res << it.getFeature<PStringer>()->toString(it);
			else
			{
				res << fmt::format("{0}<{1}>", it.getFeature<PIdentifier>()->identifier(), (void*)it.get());
			}
			if(i != std::min(int64_t(10), *l->size()) - 1) res << ", ";
		}
		res << "]";

		return res.str();

	});
	_.defaults();
}


void system::make_list_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

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

	auto get = instance<MultiMethod>::make();
	get->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List, int64_t>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<int64_t> b(expect<List>(args[1]));
		return a->at(b);
	}));
	ret->define_eval("get", get);

	auto insert = instance<MultiMethod>::make();
	insert->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		instance<int64_t> b(expect<int64_t>(args[1]));
		a->insert(b, args[2]);
		return instance<>();
	}));
	ret->define_eval("insert", insert);


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
	ret->define_eval("erase", erase);

	auto pop = instance<MultiMethod>::make();
	pop->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		return a->pop();
	}));
	ret->define_eval("pop", pop);

	auto push = instance<MultiMethod>::make();
	push->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		a->push(args[1]);
		return instance<>();
	}));
	ret->define_eval("push", push);

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
	ret->define_eval("slice", slice);

	auto reverse = instance<MultiMethod>::make();
	reverse->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<List>(),
		[](auto frame, auto args)
	{
		instance<List> a(expect<List>(args[0]));
		a->reverse();
		return instance<>();
	}));
	ret->define_eval("reverse", reverse);
}
