
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
	class Map
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Map);
	private:
		struct cmpByInstanceSemantics {
			bool operator()(const instance<>& a, const instance<>& b) const {
				if (a.typeId() != b.typeId()) return a.typeId() < b.typeId();

				if (a.typeId().isType<std::string>())
					return *a.asType<std::string>() < *b.asType<std::string>();
				else if(a.typeId().isType<int64_t>())
					return *a.asType<int64_t>() < *b.asType<int64_t>();
				else return a.get() < b.get();
			}
		};

		std::map<instance<>, instance<>, cmpByInstanceSemantics> _data;

	public:

		instance<int64_t> size()
		{
			return instance<int64_t>::make(_data.size());
		}

		instance<> at(instance<> i)
		{
			auto res = _data.find(i);
			if (res == _data.end()) return instance<>();
			return res->second;
		}

		void insert(instance<> i, instance<> v) {
			_data[i] = v;
		}

		void erase(instance<> i) {
			_data.erase(i);
		}

		std::map<instance<>, instance<>, cmpByInstanceSemantics>& data()
		{
			return _data;
		}


		std::vector<instance<>> keys() 
		{
			std::vector<instance<>> res;
			for (auto i : _data)
			{
				res.push_back(i.first);
			}
			return res;
		}

		std::vector<instance<>> values() 
		{
			std::vector<instance<>> res;
			for (auto i : _data)
			{
				res.push_back(i.second);
			}
			return res;
		}
	};
}}}

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

			if (it.second.hasFeature<PRepr>()) s = it.second.getFeature<PRepr>()->toRepr(it.second);
			else s = fmt::format("{0}<{1}>", it.second.getFeature<PIdentifier>()->identifier(), (void*)it.second.get());
			

			res << fmt::format("{0}: {1}", f, s);
			if (count != size - 1) res << ", ";

			count++;
		}
		res << "}";

		return res.str();

	});
	_.defaults();
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
	ret->define_eval("mget", get);

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
	ret->define_eval("mset", insert);


	auto erase = instance<MultiMethod>::make();
	erase->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		if (args.size() != 2) throw stdext::exception("Dispatch Failure");
		instance<Map> a(expect<Map>(args[0]));

		a->erase(args[1]);
		return instance<>();
	}));
	ret->define_eval("mclear", erase);

	auto keys = instance<MultiMethod>::make();
	keys->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Map>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Map> a(expect<Map>(args[0]));

		return frame->getNamespace()->lookup("list")->getValue(frame).asType<MultiMethod>()->call(frame, a->keys());
	}));
	ret->define_eval("keys", keys);

	auto values = instance<MultiMethod>::make();
	values->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Map>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Map> a(expect<Map>(args[0]));

		return frame->getNamespace()->lookup("list")->getValue(frame).asType<MultiMethod>()->call(frame, a->values());
	}));
	ret->define_eval("values", values);
}
