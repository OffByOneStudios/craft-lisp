#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Scope.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Scope)
{
	_.use<SScope>().byCasting();

	_.defaults();
}


Scope::Scope(instance<Environment> env)
{
	_environment = env;
	_parent = instance<>();
}

Scope::Scope(instance<SScope> parent)
{
	_environment = parent->environment();
	_parent = parent;
}


instance<Environment> Scope::environment() const
{
	return _environment;
}
instance<Namespace> Scope::namespace_() const
{
	return _parent->namespace_();
}
instance<SScope> Scope::parent() const
{
	return _parent;
}

std::vector<instance<SBinding>> Scope::search(std::string const & search)
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

instance<SBinding> Scope::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
	{
		if (_parent)
			return _parent->lookup(s);
		else
			throw stdext::exception("Lookup failed, `{0}` not in scope.", s);
	}

	return it->second;
}

instance<SBinding> Scope::define(std::string name, instance<> value)
{
	auto binding = instance<Binding>::make(name, value);
	_lookup[binding->name()] = binding;
	return binding;
}
