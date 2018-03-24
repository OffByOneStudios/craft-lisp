#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Namespace.h"

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

 instance<> Namespace::get(types::TypeId type)
 {

 }

instance<> Namespace::parse(std::string contents, types::TypeId type, PSyntax::ParseOptions const* opts = nullptr)
{

}

instance<> Namespace::read(std::string contents, types::TypeId type, PSyntax::ReadOptions const* opts = nullptr)
{

}
instance<> Namespace::read(instance<> source, types::TypeId type, PSyntax::ReadOptions const* opts = nullptr)
{

}

instance<> Namespace::exec(instance<Module> module, std::string method, lisp::GenericCall const& call = {})
{

}

void Namespace::compile(std::string path, instance<> compiler_options)
{

}
void Namespace::compile(instance<Module> module, std::string path, instance<> compiler_options)
{

}


instance<Module> Namespace::requireModule(std::string const& uri_, instance<> resolver_specific_extra)
{
	auto uri = uri_;
	auto protopos = uri.find(':');
	if (protopos == std::string::npos)
		throw stdext::exception("Malformed module request `{0}`.", uri);

	std::string protocol = uri.substr(0, protopos);
	std::string rest = uri.substr(protopos + 1);

	instance<Module> ret;

	try
	{
		// TODO implement resolvers/loaders for these
		// All of this should go somewhere else, and be made internal to Module
		if (protocol == "builtin" && rest == "cult.system")
			ret = make_library_globals(craft_instance());
		if (protocol == "repl")
		{
			ret = instance<Module>::make(craft_instance(), uri);
			ret->setLive();
		}
		if (protocol == "file")
		{
			auto s = path::normalize(path::absolute(rest));
			if (!path::exists(s))
				throw stdext::exception("Module path `{0}` does not exist.", s);

			auto text = craft::fs::read<std::string>(s, &craft::fs::string_read).get();
			ret = instance<Module>::make(craft_instance(), uri);
			ret->content = _environment->read(ret, text);
		}

		// TODO set uri to the canonical version of the uri from the resolver
		uri = uri;
	}
	catch (std::exception const& ex)
	{
		throw stdext::exception(ex, "Failed to construct module `{0}`", uri);
	}

	if (ret)
	{
		ret->load();
		ret->init();
	}

	// TODO: Lock when we do this (and the init above probably)
	auto i = _module_load_list.size();
	_module_load_list.push_back(ret);
	_module_cache[uri] = i;
	on_moduleInit.emit(ret);

	interpreter_provider->addModule(backend, ret);
	ret->backend = backend_provider->addModule(backend, ret);

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
