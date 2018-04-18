#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Namespace.h"

#include "lisp/backend/BootstrapInterpreter.h"
//#include "lisp/backend/llvm/llvm_internal.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Namespace)
{
	_.defaults();
}

Namespace::Namespace(instance<Environment> env)
{
	_environment = env;
	_loaderVar_anonCount = 0;
}

void Namespace::craft_setupInstance()
{
	Object::craft_setupInstance();

	for (auto type : PBackend::craft_s_featureManager()->supportedTypes())
	{
		auto backend = type.getFeature<PBackend>();

		_backends[type] =
		{
			backend->init(craft_instance()),
			backend,
			type.hasFeature<PExecutor>() ? type.getFeature<PExecutor>() : nullptr,
			type.hasFeature<PCompiler>() ? type.getFeature<PCompiler>() : nullptr,
		};
	}

	instance<Execution> exec = instance<Execution>::make(craft_instance());
	exec->makeCurrent();

	requireModule("builtin:cult.system")->initialize();
	requireModule("builtin:cult.core")->initialize();
}

Namespace::_Backend Namespace::preferedBackend() const
{
	return _backends.at(cpptype<BootstrapInterpreter>::typeDesc() /*cpptype<LlvmBackend>::typeDesc()*/);
}

Namespace::_Backend Namespace::fallbackBackend() const
{
	return _backends.at(cpptype<BootstrapInterpreter>::typeDesc());
}

 instance<> Namespace::get(TypeId type)
 {
	 auto it = _backends.find(type);
	 if (it == _backends.end())
		 throw bad_projection_error("Cannot get a projection to `{1}` from {0}", craft_instance(), type.toString(false));

	 return it->second.instance;
 }

instance<> Namespace::parse(std::string contents, TypeId type, PSyntax::ParseOptions const* opts)
{
	// context variable with: ParseOptions
	instance<Module> module = requireModule("anon:", instance<std::string>::make(contents));

	return module->get(type);
}

instance<> Namespace::read(instance<> source, TypeId type, PSemantics::ReadOptions const* ropts)
{
	auto module = source.getFeature<PSyntax>()->getModule(source);

	// context variable with: ReadOptions
	return module->require(type);
}

instance<> Namespace::read(std::string contents, TypeId type, PSyntax::ParseOptions const* popts, PSemantics::ReadOptions const* ropts)
{
	return read(parse(contents, type.getFeature<PSemantics>()->readsFrom()[0], popts), type, ropts);
}

instance<> Namespace::exec(instance<Module> module, std::string method, types::GenericInvoke const& call)
{
	auto backend = preferedBackend();

	return backend.executor->exec(backend.instance, module, method, call);
}

void Namespace::compile(std::string path, instance<> compiler_options)
{

}
void Namespace::compile(instance<Module> module, std::string path, instance<> compiler_options)
{

}


instance<Module> Namespace::requireModule(std::string const& uri_, instance<> resolver_specific_extra)
{
	auto mlb = _module_cache.lower_bound(uri_);
	if (mlb != _module_cache.end() && !(_module_cache.key_comp()(uri_, mlb->first)))
		return _module_load_list[mlb->second]; // key already exists

	auto uri = uri_;
	auto protopos = uri.find(':');
	if (protopos == std::string::npos)
		throw stdext::exception("Malformed module request `{0}`.", uri);

	std::string protocol = uri.substr(0, protopos);
	std::string rest = uri.substr(protopos + 1);

	instance<Module> ret;

	try
	{
		if (protocol == "anon")
		{
			rest = fmt::format("{0}--{1}", rest, std::to_string(++_loaderVar_anonCount));
		}

		auto ml = types::system().getManager<PModuleLoader>()->index(protocol);
		if (ml == nullptr)
			throw stdext::exception("Unknown module loader protocol `{0}`", protocol);

		ret = ml.getFeature<PModuleLoader>()->loadModule(craft_instance(), rest, resolver_specific_extra);
	}
	catch (std::exception const& ex)
	{
		throw module_resolve_error(ex, "Failed to construct module `{0}`", uri);
	}
	
	if (ret)
	{
		ret->load();
	}
	else
		throw module_resolve_error("Could not resolve module `{0}`", uri);

	// TODO: Lock when we do this (and the init above probably)
	auto i = _module_load_list.size();
	_module_load_list.push_back(ret);
	_module_cache.insert(mlb, { uri, i });
	on_moduleInit.emit(ret);

	return ret;
}
