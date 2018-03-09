
#include "lisp/common.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void system::make_shim_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto _new = instance<MultiMethod>::make();
	_new->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));

		auto tid = types::system().getManager<PIdentifier>()->index(*a);

		auto constructors = types::system().getManager<PConstructor>();
		if (!constructors->hasProvider(tid)) throw stdext::exception("Type {} does not implement PConstructor", *a);
		auto cons = (PConstructor*)constructors->getProvider(tid);
		return cons->construct();
		
	}));
	ret->define_eval("new", _new);

	auto _parse = instance<MultiMethod>::make();
	_parse->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0])), b(expect<std::string>(args[1]));

		auto tid = types::system().getManager<PIdentifier>()->index(*a);
		auto parsers = types::system().getManager<PParse>();
		if (!parsers->hasProvider(tid)) throw stdext::exception("Type {} does not implement PParse", *a);
		auto parse = (PParse*)parsers->getProvider(tid);
		return parse->parse(*b);

	}));
	ret->define_eval("parse", _parse);

	auto _call = instance<MultiMethod>::make();
	_call->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<> a(args[0]);
		instance<std::string> b(expect<std::string>(args[1]));

		if (!a.hasFeature<SObjectManipulation>()) throw stdext::exception("Object Does not Implement SObjectManipulation");

		auto mani = a.getFeature<SObjectManipulation>();

		auto listings = mani->objectManipulation_findListings(*b);

		if(!listings.size()) throw stdext::exception("Object Has no Such Member {0}", *b.get());

		for (auto l : listings)
		{
			auto meta = mani->objectManipulation_getMeta(l);
			if (meta.argTypes.size() != args.size() - 2) continue;

			bool found = true;
			for (size_t i = 0; i != meta.argTypes.size(); i++)
			{
				if (args[i + 2].typeId() != meta.argTypes[i])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				auto cargs = std::vector<instance<>>(args.begin() + 2, args.end());

				return mani->objectManipulation_call(l, cargs);
			}
		}
		
		throw stdext::exception("Object Has no Member {0} Matching Argument List", *b.get());

	}));
	ret->define_eval("call", _call);


	auto ctypes = instance<MultiMethod>::make();
	ctypes->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		std::vector<std::string> res;
		auto ids = types::system().getManager<PIdentifier>();
		for (auto tid : ids->supportedTypes())
		{
			auto p = ((PIdentifier*)ids->getProvider(tid))->identifier();
			
			res.push_back(p);
		}
		return instance<std::string>::make(stdext::join(' ', res.begin(), res.end()));
	}));
	ret->define_eval("ctypes", ctypes);
}
