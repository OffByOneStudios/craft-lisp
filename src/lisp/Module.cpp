#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Module.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Module)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Module> m) { return m->filepath(); });

	_.use<SScope>().byCasting();

	_.defaults();
}


Module::Module(instance<Namespace> ns, std::string filepath)
	: _environment(), _ns(), _filepath(), _lookup()
{
	_environment = ns->environment();
	_ns = ns;
	_filepath = filepath;
}

instance<Environment> Module::environment() const
{
	return _environment;
}
std::string Module::filepath() const
{
	return _filepath;
}
instance<Namespace> Module::namespace_() const
{
	return _ns;
}

instance<Binding> Module::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
	{
		return _ns->lookup(s);
	}

	return it->second;
}

void Module::define(instance<Binding> binding)
{
	binding->addMeta("module", craft_instance_from_this());
	_lookup[binding->name()] = binding;
	_ns->define(binding);
}

void Module::load()
{
	auto text = craft::fs::read<std::string>(_filepath, &craft::fs::string_read).get();

	auto env = _ns->environment();

	instance<Sexpr> cell = env->read(craft_instance_from_this(), text);
	for (auto c : cell->cells)
	{
		env->eval(craft_instance_from_this(), c);
	}
}
