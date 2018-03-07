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

	define_eval("*module*", craft_instance_from_this());
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

std::vector<instance<SBinding>> const& Module::bindings() const
{
	return _bindings;
}

instance<SBinding> Module::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
	{
		return _ns->lookup(s);
	}

	return _bindings[it->second];
}

instance<SBinding> Module::define(std::string name, instance<> value)
{
	auto binding = instance<Binding>::make(name, value);
	binding->addMeta("module", craft_instance_from_this());
	auto i = _bindings.size();
	_bindings.push_back(binding);
	_lookup[binding->name()] = i;
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

void Module::setLive()
{
	content = instance<Sexpr>::make();
}

void Module::load()
{
	// TODO call loader
}

void Module::init()
{
	auto env = _ns->environment();

	auto frame = instance<Frame>::make(craft_instance_from_this());
	Execution::execute(frame);

	if (content)
		for (auto c : content->cells)
		{
			env->eval(frame, c);
		}
}

instance<> Module::liveContinueWith(instance<Sexpr> parsed_code)
{
	auto env = _ns->environment();
	
	instance<Sexpr> read_code = env->read(craft_instance_from_this(), parsed_code);

	auto frame = instance<Frame>::make(craft_instance_from_this());
	Execution::execute(frame);

	instance<> last_result;
	for (auto c : read_code->cells)
	{
		last_result = env->eval(frame, c);
	}

	return last_result;
}
