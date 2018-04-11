
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

CRAFT_OBJECT_DEFINE(Map)
{
	_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
		->withMethod("size", &Map::size)
		->withMethod("at", &Map::at)
		->withMethod("erase", &Map::erase)
		->withMethod("insert", &Map::insert);

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<Map> l) -> std::string
	{
		std::ostringstream res;

		auto count = 0;
		auto size = *l->size();

		res << "{";
		for (auto it : l->data())
		{
			std::string f, s;
			if (it.first.hasFeature<PRepr>()) f = it.first.getFeature<PRepr>()->toRepr(it.first);
			else f = fmt::format("{0}<{1}>", it.first.getFeature<PIdentifier>()->identifier(), (void*)it.first.get());

			if (it.second.get() == nullptr) s = "null";
			else if (it.second.hasFeature<PRepr>()) s = it.second.getFeature<PRepr>()->toRepr(it.second);
			else s = fmt::format("{0}<{1}>", it.second.getFeature<PIdentifier>()->identifier(), (void*)it.second.get());


			res << fmt::format("{0}: {1}", f, s);
			if (count != size - 1) res << ", ";

			count++;
		}
		res << "}";

		return res.str();

	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](instance<Map> l) -> std::string
	{
		return l.getFeature<PStringer>()->toString(l);
	});
	_.defaults();
}

instance<int64_t> Map::size()
{
	return instance<int64_t>::make(_data.size());
}

instance<> Map::at(instance<> i)
{
	auto res = _data.find(i);
	if (res == _data.end()) return instance<>();
	return res->second;
}

void Map::insert(instance<> i, instance<> v) {
	_data[i] = v;
}

void Map::erase(instance<> i) {
	_data.erase(i);
}

std::map<instance<>, instance<>, Map::cmpByInstanceSemantics>& Map::data()
{
	return _data;
}


std::vector<instance<>> Map::keys()
{
	std::vector<instance<>> res;
	for (auto i : _data)
	{
		res.push_back(i.first);
	}
	return res;
}

std::vector<instance<>> Map::values()
{
	std::vector<instance<>> res;
	for (auto i : _data)
	{
		res.push_back(i.second);
	}
	return res;
}

void system::make_map_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto map = instance<MultiMethod>::make();
	map->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		if (args.size() % 2 != 0) throw stdext::exception("Arguments must be in key value order");
		auto res = instance<Map>::make();
		for (auto it = 0; it < args.size(); it += 2)
		{
			res->insert(args[it], args[it + 1]);
		}
		return res;
	}));
	ret->define_eval("map", map);

	auto get = instance<MultiMethod>::make();
	get->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		if (args.size() != 2) throw stdext::exception("Dispatch Failure");
		instance<Map> a(expect<Map>(args[0]));
		return a->at(args[1]);
	}));
	ret->define_eval("map/get", get);

	auto insert = instance<MultiMethod>::make();
	insert->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		if ((args.size() - 1) % 2 != 0) throw stdext::exception("Arguments must be in key value order");

		instance<Map> a(expect<Map>(args[0]));
		for (auto it = 1; it < args.size(); it += 2)
		{
			a->insert(args[it], args[it + 1]);
		}
		return instance<>();
	}));
	ret->define_eval("map/set", insert);


	auto erase = instance<MultiMethod>::make();
	erase->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		if (args.size() != 2) throw stdext::exception("Dispatch Failure");
		instance<Map> a(expect<Map>(args[0]));

		a->erase(args[1]);
		return instance<>();
	}));
	ret->define_eval("map/clear", erase);

	auto keys = instance<MultiMethod>::make();
	keys->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Map>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Map> a(expect<Map>(args[0]));

		return frame->getNamespace()->lookup("list")->getValue(frame).asType<MultiMethod>()->call(frame, a->keys());
	}));
	ret->define_eval("map/keys", keys);

	auto values = instance<MultiMethod>::make();
	values->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Map>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Map> a(expect<Map>(args[0]));

		return frame->getNamespace()->lookup("list")->getValue(frame).asType<MultiMethod>()->call(frame, a->values());
	}));
	ret->define_eval("map/values", values);


	auto fmap = instance<MultiMethod>::make();
	fmap->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Map, Function>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Map> a(expect<Map>(args[0]));
		instance<Function> b(expect<Function>(args[1]));

		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto call = instance<Sexpr>::make();
			call->cells = { b, i.first, i.second, count };
			res->push(frame->getNamespace()->environment()->eval(frame, call));
			(*count)++;
		}
		return res;
	}));
	fmap->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Map, Closure>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Map> a(expect<Map>(args[0]));
		instance<Closure> b(expect<Closure>(args[1]));

		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			auto call = instance<Sexpr>::make();
			call->cells = { b, i.first, i.second, count };
			res->push(frame->getNamespace()->environment()->eval(frame, call));
			(*count)++;
		}
		return res;
	}));
	ret->define_eval("map/fmap", fmap);
}
