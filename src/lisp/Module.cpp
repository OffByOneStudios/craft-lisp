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


Module::Module(instance<Namespace> ns, std::string uri)
{
	_ns = ns;
	_uri = uri;
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

bool Module::isLoaded() const
{
	return _loader;
}

void Module::load()
{
	// TODO call loader
	// For the time being, assume the loader is the syntax structure, and that the Namespace has set it for us
	_syntax_instance = _loader;
	_syntax_syntax = _syntax_instance.getFeature<PSyntax>();
}

bool Module::isInitialized() const
{
	return _value;
}
void Module::initialize()
{
	if (isInitialized())
		return;

	_value = exec("$init", { craft_instance() });
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
	_value = exec("$append", { moduleToAppend });
}
void Module::mergeModule(instance<lisp::Module> moduleToMerge)
{
	_value = exec("$merge", { moduleToMerge });
}

instance<> Module::get(types::TypeId type)
{
	if (_syntax_instance.isType(type))
		return _syntax_instance;

	auto it = _semantics.find(type);
	if (it != _semantics.end())
		return it->second.instance;

	return instance<>();
}
instance<> Module::require(types::TypeId type, bool force_read)
{
	auto ret = get(type);

	if (!ret)
	{
		if (type.hasFeature<PSemantics>())
		{
			PSemantics* semantics = type.getFeature<PSemantics>();

			// Search for read sources
			auto read_targets = semantics->readsFrom();
			std::vector<instance<>> valid_read_targets;

			for (auto target : read_targets)
			{
				instance<> read_target;
				try
				{
					read_target = get(target);
				}
				catch (bad_projection_error const&)
				{
					continue;
				}

				valid_read_targets.push_back(read_target);
			}

			// Search for transform targets
			// TODO

			// Choose
			instance<> target;
			bool isRead = true;

			if (valid_read_targets.size() == 1) target = valid_read_targets[0];

			// Report error
			if (!target && force_read)
				throw bad_projection_error("Cannot require a projection to `{1}` from {0}: no valid path to transform/read semantics from.", craft_instance(), type.toString(false));

			// Perform
			if (target)
			{
				instance<> semantics_instance;
				try
				{
					semantics_instance = semantics->read(target, craft_instance(), nullptr);
				}
				catch (std::exception const& ex)
				{
					throw bad_projection_error(ex, "Cannot require a projection to `{1}` from {0}, failed reading", craft_instance(), type.toString(false));
				}

				_semantics[type] = { semantics_instance, semantics };
			}
		}

		throw bad_projection_error("Cannot require a projection to `{1}` from {0}: type does not implement PSemantics.", craft_instance(), type.toString(false));
	}

	return ret;
}

instance<> Module::exec(std::string method, types::GenericInvoke const& call)
{
	return _ns->exec(craft_instance(), method, call);
}

void Module::builtin_setSemantics(instance<> semantics)
{
	// TODO build a syntax "representation" of the semantics
	// TODO fill in the loader correctly
	_loader = semantics;

	_semantics[semantics.typeId()] = { semantics, semantics.getFeature<PSemantics>() };
}
