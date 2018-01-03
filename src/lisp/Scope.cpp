#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Scope.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Scope)
{
	_.use<SScope>().byCasting();

	_.defaults();
}


Scope::Scope(instance<Environment> env, instance<SScope> parent)
{
	_environment = env;
	_parent = parent;
}

instance<Environment> Scope::environment() const
{
	return _environment;
}
instance<SScope> Scope::parent() const
{
	return _parent;
}

instance<Binding> Scope::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
		throw stdext::exception("Lookup failed, `{0}` not in scope.", s);

	return it->second;
}

void Scope::define(instance<Binding> binding)
{
	_lookup[binding->name()] = binding;
}
