#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Namespace.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Namespace)
{
	_.use<SScope>().byCasting();

	_.defaults();
}


Namespace::Namespace(instance<Environment> env)
{
	_environment = env;

	define(instance<Binding>::make("*ns*", craft_instance_from_this()));
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
			ret = make_library_globals(craft_instance_from_this());
		if (protocol == "repl")
		{
			ret = instance<Module>::make(craft_instance_from_this(), uri);
			ret->setLive();
		}
		if (protocol == "file")
		{
			auto s = path::normalize(path::absolute(rest));
			if (!path::exists(s))
				throw stdext::exception("Module path `{0}` does not exist.", s);

			auto text = craft::fs::read<std::string>(s, &craft::fs::string_read).get();
			ret = instance<Module>::make(craft_instance_from_this(), uri);
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

instance<Environment> Namespace::environment() const
{
	return _environment;
}
instance<Namespace> Namespace::namespace_() const
{
	return craft_instance_from_this();
}
instance<SScope> Namespace::parent() const
{
	return instance<SScope>();
}

std::vector<instance<SBinding>> Namespace::search(std::string const & search)
{
	std::vector<instance<SBinding>> res;
	for (auto& it : _lookup)
	{
		if (it.first.find(search) != it.first.npos)
		{
			res.push_back(it.second);
		}
	}
	return res;

}

instance<SBinding> Namespace::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
		throw stdext::exception("Lookup failed, `{0}` not in scope.", s);

	return it->second;
}

instance<SBinding> Namespace::define(std::string name, instance<> value)
{
	return define(instance<Binding>::make(name, value));
}

instance<SBinding> Namespace::define(instance<SBinding> binding)
{
	_lookup[binding->name()] = binding;
	return binding;
}

