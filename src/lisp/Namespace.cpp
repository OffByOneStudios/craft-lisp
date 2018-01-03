#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Namespace.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Namespace)
{
	_.defaults();

	_.use<SScope>().byCasting();
}


Namespace::Namespace(instance<Environment> env)
{
	_environment = env;

	define(instance<Binding>::make("*ns*", craft_instance_from_this()));
}

instance<Environment> Namespace::environment() const
{
	return _environment;
}

instance<Binding> Namespace::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
		throw stdext::exception("Lookup failed, `{0}` not in scope.", s);

	return it->second;
}

void Namespace::define(instance<Binding> binding)
{
	_lookup[binding->name()] = binding;
}

