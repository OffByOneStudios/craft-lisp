#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Module.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Module)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Module> m) { return m->uri(); });

	_.defaults();
}


Module::Module(instance<Namespace> ns, instance<> loader)
{
	_ns = ns;
	_loader = loader;
	_uri = _loader.getFeature<PModuleLoader>()->getUri(_loader);

	_state = State::HasLoader;
}

void Module::craft_setupInstance()
{
	Object::craft_setupInstance();

	//define_eval("*module*", craft_instance());
}

std::string Module::uri() const
{
	return _uri;
}

instance<Namespace> Module::getNamespace() const
{
	return _ns;
}

instance<> Module::getLoader() const
{
	return _loader;
}


Module::State Module::getState() const
{
	return _state;
}

bool Module::hasLoader() const
{
	assert((_state & State::FLAG_HasLoader) == _loader);

	return _state & State::FLAG_HasLoader;
}
bool Module::isLoaded() const
{
	assert((_state & State::FLAG_IsLoaded) == _syntax_instance);

	return _state & State::FLAG_IsLoaded;
}
bool Module::isInitializing() const
{
	return _state & State::FLAG_IsInitializing;
}
bool Module::wasInitialized() const
{
	assert((_state & State::FLAG_WasInitialized) == _value);

	return _state & State::FLAG_WasInitialized;
}
bool Module::isReady() const
{
	assert((_state == State::Ready) == _value);

	return _state == State::Ready;
}


bool Module::load()
{
	if (isLoaded()) return true;

	{
		std::unique_lock<std::mutex> lock(_mutex);

		_syntax_instance = _loader.getFeature<PModuleLoader>()->getContent(_loader);
		_syntax_syntax = _syntax_instance.getFeature<PSyntax>();

		_state = State::Loaded;
	}

	return true;
}
bool Module::initialize()
{
	if (isReady()) return true;
	if (isInitializing()) return false;

	{
		std::unique_lock<std::mutex> lock(_mutex);
		_state = State::Initializing;

		_value = exec("::init", {});

		_state = State::Ready;
	}

	return true;
}
bool Module::update(bool force)
{
	bool needsUpdate = force;

	if (!force)
	{
		getNamespace()->getEnvironment()->log()->warn("Detecting updates isn't implemented yet; detect yourself and call with force.");
	}

	if (needsUpdate)
	{
		if (isInitializing()) return false;

		std::unique_lock<std::mutex> lock(_mutex);
		_state = State::ReInitializing;

		_syntax_instance = _loader.getFeature<PModuleLoader>()->getContent(_loader);
		_syntax_syntax = _syntax_instance.getFeature<PSyntax>();

		_value = exec("::init", {});

		_state = State::Ready;
	}

	return true;
}

instance<> Module::moduleValue() const
{
	return _value;
}

instance<> Module::lastExecutedResult() const
{
	return _lastResult;
}

void Module::appendModule(instance<lisp::Module> moduleToAppend)
{
	_lastResult = exec("::append", { moduleToAppend });
}
void Module::mergeModule(instance<lisp::Module> moduleToMerge)
{
	_lastResult = exec("::merge", { moduleToMerge });
}

instance<> Module::get(types::TypeId type)
{
	if (_syntax_instance.isType(type))
		return _syntax_instance;

	auto it = _semantics.find(type);
	if (it != _semantics.end())
		return it->second.inst;

	return instance<>();
}
instance<> Module::require(types::TypeId type)
{
	auto ret = get(type);

	if (!ret)
	{
		if (!type.hasFeature<PSemantics>())
			throw bad_projection_error("Cannot require a projection to `{1}` from {0}:\ntype does not implement PSemantics.", craft_instance(), type.toString(false));

		PSemantics* semantics = type.getFeature<PSemantics>();

		instance<> semantics_instance;
		try
		{
			semantics_instance = semantics->read(craft_instance(), nullptr);
		}
		catch (std::exception const& ex)
		{
			throw bad_projection_error(ex, "Cannot require a projection to `{1}` from {0}, failed reading", craft_instance(), type.toString(false));
		}
		_semantics[type] = { semantics_instance, semantics };

		return semantics_instance;
	}

	return ret;
}

instance<> Module::exec(std::string method, types::GenericInvoke const& call)
{
	return _ns->exec(craft_instance(), method, call);
}

instance<> Module::eval(std::string const& code)
{
	// TODO
	return instance<>();
}

void Module::builtin_setSemantics(instance<> semantics)
{
	// TODO build a syntax "representation" of the semantics (let the builtin loaders do it?)
	_semantics[semantics.typeId()] = { semantics, semantics.getFeature<PSemantics>() };
}
