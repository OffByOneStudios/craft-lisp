#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Module.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Module)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Module> m) { return m->uri(); });

	_.use<SScope>().byCasting();

	_.defaults();
}


Module::Module(instance<Namespace> ns, std::string uri)
{
	_environment = ns->environment();
	_ns = ns;
	_uri = uri;
	_inited = false;
}

std::string Module::uri() const
{
	return _uri;
}

instance<Environment> Module::environment() const
{
	return _environment;
}
instance<Namespace> Module::namespace_() const
{
	return _ns;
}
instance<SScope> Module::parent() const
{
	return _ns;
}

instance<SBinding> Module::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
	{
		return _ns->lookup(s);
	}

	return it->second;
}

instance<SBinding> Module::define(std::string name, instance<> value)
{
	auto binding = instance<Binding>::make(name, value);
	binding->addMeta("module", craft_instance_from_this());
	_lookup[binding->name()] = binding;
	_ns->define(binding);
	return binding;
}

instance<SBinding> Module::define_eval(std::string name, instance<> value)
{
	auto res = define(name, value);

	res.asType<Binding>()->setValue(value);

	return res;
}

bool Module::isLoaded() const
{
	return bool(content);
}
bool Module::isInitalized() const
{
	return _inited;
}

void Module::load()
{
	// TODO call loader
	//auto text = craft::fs::read<std::string>(_uri, &craft::fs::string_read).get();

	auto env = _ns->environment();

	//content = env->read(craft_instance_from_this(), text);
}

void Module::init()
{
	auto env = _ns->environment();

	auto execution = instance<Execution>::make(env, _ns);
	auto frame = instance<Frame>::make(instance<Frame>::make(execution), craft_instance_from_this());

	if (content)
		for (auto c : content->cells)
		{
			env->eval(frame, c);
		}
}
