
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

CRAFT_DEFINE(Map)
{
	//_.use<SObjectManipulation>().byConfiguring<ObjectManipulater>()
	//	->withMethod("size", &Map::size)
	//	//->withMethod("at", &Map::at)
	//	//->withMethod("erase", &Map::erase)
	//	->withMethod("clear", &Map::clear)
	//	//->withMethod("insert", &Map::insert)
	//	;

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
			else f = fmt::format("{0}<{1}>", it.first.typeId(), (void*)it.first.get());

			if (it.second.get() == nullptr) s = "null";
			else if (it.second.hasFeature<PRepr>()) s = it.second.getFeature<PRepr>()->toRepr(it.second);
			else s = fmt::format("{0}<{1}>", it.second.typeId(), (void*)it.second.get());


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


Map::Map(std::map<instance<>, instance<>, cmpByInstanceSemantics> && data)
	: _data(data)
{
	
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

void Map::clear()
{
	_data.clear();
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

void core::make_map_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_defineType<Map>("Map");

	semantics->builtin_implementMultiMethod("map",
		[](types::VarArgs<instance<>> args) -> instance<Map>
	{
		if (args.args.size() % 2 != 0) throw stdext::exception("Arguments must be in key value order");
		auto res = instance<Map>::make();
		for (auto it = 0; it < args.args.size(); it += 2)
		{
			res->insert(args.args[it], args.args[it + 1]);
		}
		return res;
	});

	semantics->builtin_implementMultiMethod("map/get",
		[](instance<Map> a, instance<> b) -> instance<>
	{
		return a->at(b);
	});
	
	semantics->builtin_implementMultiMethod("map/set",
		[](instance<Map> a, instance<> b, instance<> c)
	{
		a->insert(b, c);
	});
	
	semantics->builtin_implementMultiMethod("map/erase",
		[](instance<Map> a, instance<> b)
	{
		a->erase(b);
	});

	semantics->builtin_implementMultiMethod("map/clear",
		[](instance<Map> a)
	{
		a->clear();
	});

	semantics->builtin_implementMultiMethod("map/keys",
		[](instance<Map> a) -> instance<List>
	{
		return instance<List>::make(a->keys());
	});

	semantics->builtin_implementMultiMethod("map/values",
		[](instance<Map> a) -> instance<List>
	{
		return instance<List>::make(a->values());
	});

	semantics->builtin_implementMultiMethod("map/fmap",
		[](instance<Map> a, instance<Function> b) -> instance<List>
	{
		auto res = instance<List>::make();

		auto count = instance<int64_t>::make(0);

		for (auto& i : a->data())
		{
			res->push(Execution::exec(b, { i.first, i.second, count }));
			(*count)++;
		}

		return res;
	});
}
