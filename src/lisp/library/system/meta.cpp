
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void core::make_meta_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("craft-type",
		[](instance<std::string> s) -> instance<types::Graph::Node>
	{
		return instance<types::Graph::Node>::makeFromPointerAndMemoryManager(graph().getByIndex<GraphPropertyTypeName>(s.get()), &graph());
	});

	semantics->builtin_implementMultiMethod("cpp-type",
		[](instance<std::string> s) -> instance<types::Graph::Node>
	{
		return instance<types::Graph::Node>::makeFromPointerAndMemoryManager(graph().getByIndex<GraphPropertyCppName>(s->c_str()), &graph());
	});

	semantics->builtin_implementMultiMethod("new",
		[](instance<types::Graph::Node> type, types::VarArgs<instance<>> args)
	{
		if (type->getInterface()->name == std::string("cult.type"))
		{
			auto slotcount = graph().getFirstPropValue<GraphPropertyCppSize>(type.get()) / 8;
			auto res = instance<RuntimeSlots>::make(instance<>(), slotcount);
			auto header = (types::InstanceHeader*)res.asInternalPointer();
			header->typeId = type.get();

			return res;
		}
		// TODO Use PConstructor
		throw stdext::exception("Branch Not Implemented, {0}", (types::TypeId)type.get());
	});

	semantics->builtin_implementMultiMethod("dump",
		[](instance<types::Graph::Node> s) -> instance<std::string>
	{
		if (!s) return instance<>();
		return instance<std::string>::make(graph().dumpNode(s.get()));
	});

	semantics->builtin_implementMultiMethod("builtins",
		[]() -> instance<std::string>
	{
		auto ret = instance<std::string>::make("Builtins:");
		auto res = graph().listIndex<GraphPropertyBuiltinModuleUri>();

		for (auto n : res)
		{
			*ret += "\n\t";
			*ret += graph().getFirstPropValue<GraphPropertyBuiltinModuleUri>(n);
		}
		return ret;
	});

	semantics->builtin_implementMultiMethod("load-dll",
		[](instance<std::string> s)
	{
#ifdef  _WIN32
		auto clean = *s + ".dll";
#elif	__APPLE__
		auto clean = "lib + *s + ".dylib";
#else
		auto clean = "lib + *s + ".so";
#endif
		types::load_dll(clean);
	});

}
