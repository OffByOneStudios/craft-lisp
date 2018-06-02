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

	requireModule(instance<>(), "builtin:cult/system")->initialize();
	requireModule(instance<>(), "builtin:cult/core")->initialize();
}

instance<Environment> Namespace::getEnvironment() const
{
	return _environment;
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

	 return it->second.inst;
 }

instance<> Namespace::parse(std::string contents, TypeId type, PSyntax::ParseOptions const* opts)
{
	// context variable with: ParseOptions
	instance<Module> module = requireModule(instance<>(), "anon:", instance<std::string>::make(contents));

	return module->get(type);
}

instance<> Namespace::read(instance<> source, TypeId type, PSemantics::ReadOptions const* ropts)
{
	auto module = source.getFeature<PSyntax>()->getModule(source);

	// context variable with: ReadOptions
	return module->require(type);
}

instance<> Namespace::read(std::string contents, TypeId parseType, types::TypeId readType, PSyntax::ParseOptions const* popts, PSemantics::ReadOptions const* ropts)
{
	return read(parse(contents, parseType, popts), readType, ropts);
}

instance<> Namespace::exec(instance<Module> module, std::string method, types::GenericInvoke const& call)
{
	auto backend = preferedBackend();

	return backend.executor->exec(backend.inst, module, method, call);
}

void Namespace::compile(std::string path, instance<> compiler_options)
{

}
void Namespace::compile(instance<Module> module, std::string path, instance<> compiler_options)
{

}


instance<Module> Namespace::requireModule(instance<Module> requestingModule, std::string const& uri_, instance<> resolver_specific_extra)
{
	std::unique_lock<std::recursive_mutex> lock(_module_load_mutex);

	auto mclb = _module_cache.lower_bound(uri_);
	if (mclb != _module_cache.end() && !(_module_cache.key_comp()(uri_, mclb->first)))
		return _module_load_list[mclb->second]; // key already exists

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

		auto mlt = types::system().getManager<PModuleLoader>()->index(protocol);
		if (mlt == nullptr)
			throw stdext::exception("Unknown module loader protocol `{0}`", protocol);
		auto ml = mlt.getFeature<PModuleLoader>();

		rest = ml->resolveProtoString(craft_instance(), requestingModule, rest, resolver_specific_extra);
		uri = fmt::format("{0}:{1}", protocol, rest);

		mclb = _module_cache.lower_bound(uri);
		if (mclb != _module_cache.end() && !(_module_cache.key_comp()(uri, mclb->first)))
			return _module_load_list[mclb->second]; // key already exists

		ret = ml->loadModule(craft_instance(), rest, resolver_specific_extra);
	}
	catch (std::exception const& ex)
	{
		throw module_resolve_error(ex, "Failed to construct module `{0}`", uri);
	}
	
	if (ret)
	{
		assert(uri == ret->uri()); // `mclb is only valid to insert with later if this is true

		uri = ret->uri();
		ret->load();
	}
	else
		throw module_resolve_error("Could not resolve module `{0}`", uri);

	// TODO: Lock when we do this (and the init above probably)
	auto i = _module_load_list.size();
	_module_load_list.push_back(ret);
	_module_cache.insert(mclb, { uri, i });
	on_moduleInit.emit(ret);

	return ret;
}
