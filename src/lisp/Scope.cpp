#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Scope.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Scope)
{
	_.defaults();
}


Scope::Scope(instance<Environment> env, instance<Scope> parent)
{
	_environment = env;
	_parent = parent;
}

instance<Environment> Scope::environment() const
{
	return _environment;
}
instance<Scope> Scope::parent() const
{
	return _parent;
}

instance<> Scope::lookup(std::string const& s)
{
	auto it = _namespace.find(s);
	if (it == _namespace.end())
		throw stdext::exception("Lookup failed, `{0}` not in scope.", s);

	return it->second;
}

void Scope::def(std::string const& s, instance<> i)
{
	_namespace[s] = i;
}
