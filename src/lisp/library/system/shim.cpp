#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "lisp/library/system/prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

void core::make_shim_globals(instance<Module> ret)
{
	/*
	auto semantics = ret->require<CultSemantics>();

	/*semantics->builtin_implementMultiMethod("types",
		[]() -> instance<List>
	{
		std::vector<instance<>> res;
		for (auto t : types::system().getManager<PIdentifier>()->supportedTypes())
		{
			res.push_back(instance<std::string>::make(t.getFeature<PIdentifier>()->identifier()));
		}
		return instance<List>::make(res);
	});

	semantics->builtin_implementMultiMethod("types/new",
		[](instance<std::string> a) -> instance<>
	{
		std::vector<instance<>> res;
		auto ids = types::system().getManager<PIdentifier>()->supportedTypes();
		bool found = false;
		cpp::TypePtr it;
		for (auto tid : types::system().getManager<PIdentifier>()->supportedTypes())
		{
			auto tname = tid.getFeature<PIdentifier>()->identifier();
			if (*a == tname)
			{
				it = tid;
				found = true;
				break;
			}
		}
		if (!found) throw stdext::exception("No Such Type {0}", *a);

		if(!(it).hasFeature<PConstructor>()) throw stdext::exception("Object {0} does not implement PConstructor", *a);

		return (it).getFeature<PConstructor>()->construct();
	});

	semantics->builtin_implementMultiMethod("types/call",
		[](types::VarArgs<instance<>> args) -> instance<>
	{
		auto target = args.args[0];
		auto method = args.args[1];
		if (!target.hasFeature<SObjectManipulation>()) 
		{
			throw stdext::exception("Target does not implement SObjectManipulation");
		}
		if (!method.isType<std::string>())
		{
			throw stdext::exception("Second Arugment must be string name of method");
		}
		auto manipulator = target.asFeature<SObjectManipulation>();


		auto listings = manipulator->objectManipulation_findListings(*method.asType<std::string>());
		auto cargs = std::vector<instance<>>(args.args.begin() + 2, args.args.end());
		for (auto l : listings)
		{
			auto meta = manipulator->objectManipulation_getMeta(l);
			if (meta.argTypes.size() != cargs.size()) continue;
			bool found = true;
			for (auto argCount = 0; argCount < meta.argTypes.size(); argCount++)
			{
				auto m = meta;
				if (meta.argTypes[argCount].id != cargs[argCount].typeId().id)
				{
					found = false;
					break;
				}
			}
			if (!found || meta.returnType.id) continue;

			return manipulator->objectManipulation_call(l, cargs);
		}

		throw stdext::exception("No Matching Manipulator for method");
	});*/
}
