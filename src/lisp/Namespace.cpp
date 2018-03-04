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

instance<Module> Namespace::requireModule(std::string const& uri, instance<> resolver_specific_extra)
{
	auto protopos = uri.find(':');
	if (protopos == std::string::npos)
		throw stdext::exception("Malformed module request `{0}`.", uri);

	std::string protocol = uri.substr(0, protopos);
	std::string rest = uri.substr(protopos + 1);

	instance<Module> ret;

	try
	{
		// TODO implement resolvers
		if (protocol == "builtin" && rest == "cult.system")
			ret = make_library_globals(craft_instance_from_this());
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

