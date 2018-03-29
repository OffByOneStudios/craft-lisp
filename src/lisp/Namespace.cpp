#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Namespace.h"

#include "lisp/backend/BootstrapInterpreter.h"
#include "lisp/backend/llvm/llvm_internal.h"
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
			type.getFeature<PExecutor>(),
			type.getFeature<PCompiler>(),
		};
	}

	// define(instance<Binding>::make("*ns*", craft_instance()));
}

Namespace::_Backend Namespace::preferedBackend() const
{
	return _backends.at(cpptype<LlvmBackend>::typeDesc());
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

	return module->_syntax_instance;
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

instance<> Namespace::exec(instance<Module> module, std::string method, lisp::GenericCall const& call)
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
		// HACK: for the time being we assume cult syntax:
		auto syntax = cpptype<CultLispSyntax>::typeDesc().getFeature<PSyntax>();

		// TODO set uri to the canonical version of the uri from the resolver
		uri = uri;

		// TODO implement resolvers/loaders for these
		// All of this should go somewhere else, and be made internal to Module
		if (protocol == "builtin")
		{
			if (rest == "cult.system")
				ret = library::make_module_builtin_cult_system(craft_instance());
			else if (rest == "cult.core")
				ret = library::make_module_builtin_cult_core(craft_instance());
		}
		else if (protocol == "repl")
		{
			ret = instance<Module>::make(craft_instance(), uri);
			ret->_loader = syntax->parse("", ret, nullptr); // hack
		}
		else if (protocol == "file")
		{
			auto s = path::normalize(path::absolute(rest));
			if (!path::exists(s))
				throw stdext::exception("Module path `{0}` does not exist.", s);

			auto text = craft::fs::read<std::string>(s, &craft::fs::string_read).get();
			ret = instance<Module>::make(craft_instance(), uri);
			ret->_loader = syntax->parse(text, ret, nullptr); // hack
		}
		else if (protocol == "anon")
		{
			uri = uri + std::to_string(++_loaderVar_anonCount);

			ret = instance<Module>::make(craft_instance(), uri);
			ret->_loader = syntax->parse(*resolver_specific_extra.asType<std::string>(), ret, nullptr); // hack
		}
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

std::vector<instance<SBinding>> Namespace::search(std::string const & search)
{
	std::vector<instance<SBinding>> res;
	for (auto& it : _lookup)
	{

		if (search.size() <= it.first.size())
		{
			if (search == it.first.substr(0, search.size()))
			{
				res.push_back(it.second);
			}
		}
	}
	return res;
}
