#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/loaders.h"

#include "lisp/semantics/cult/cult.h"
#include "lisp/library/libraries.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(PModuleLoader)
{
	_.defaults();
}



CRAFT_DEFINE(BuiltinLoader)
{
	_.use<PModuleLoader>().singleton<AutoModuleLoader>();

	_.defaults();
}

std::string BuiltinLoader::resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra)
{
	return proto_string;
}

instance<Module> BuiltinLoader::load(instance<Namespace> ns, std::string const& proto_string, instance<> extra)
{
	auto ret = instance<BuiltinLoader>::makeThroughLambda([](auto p) { return new (p) BuiltinLoader(); });
	ret->_builtinName = proto_string;

	auto builtin_descriptor = graph().getByIndex<GraphPropertyBuiltinModuleUri>(proto_string.c_str());

	if (builtin_descriptor != nullptr)
	{
		auto sd = (cpp::static_desc*)builtin_descriptor->value;
		auto bmd = (BuiltinModuleDescription*)sd->repr;

		return bmd->build(ns, ret);
	}
	
	throw stdext::exception("Unknown builtin `{0}`", proto_string);
}

instance<> BuiltinLoader::getContent()
{
	return instance<>(); // TODO generate a "syntax" for the builtins
}
std::string BuiltinLoader::getUri()
{
	return fmt::format("{0}:{1}", c_provider_index, _builtinName);
}
bool BuiltinLoader::prepSemantics(instance<>)
{
	return false;
}

instance<Module> BuiltinModuleDescription::build(instance<Namespace> ns, instance<> loader)
{
	if (_builder == nullptr)
	{
		auto ret = instance<Module>::make(ns, loader);

		auto sem = instance<CultSemantics>::make(ret);
		ret->builtin_setSemantics(sem);

		_initer(ret);

		ret->builtin_setSemantics(ret->require<CultSemantics>());

		return ret;
	}
	return _builder(ns, loader);
}

CRAFT_INIT_PRIORITY BuiltinModuleDescription craft::lisp::BuiltinCultSystem("cult.system", library::make_module_builtin_cult_system);
CRAFT_INIT_PRIORITY BuiltinModuleDescription craft::lisp::BuiltinCultCore("cult.core", library::make_module_builtin_cult_core);


CRAFT_DEFINE(FileLoader)
{
	_.use<PModuleLoader>().singleton<AutoModuleLoader>();

	_.defaults();
}

std::string FileLoader::resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra)
{
	if (requester)
	{
		auto reqLoader = requester->getLoader();
		if (reqLoader.isType<FileLoader>())
		{
			instance<FileLoader> reqFileLoader = reqLoader;
			auto relativePath = path::normalize(path::join(path::dir(reqFileLoader->_filePath), proto_string));

			if (path::exists(relativePath))
				return relativePath;

			SPDLOG_TRACE(ns->getEnvironment()->log(), "Relative path `{0}` not found using absolute path.", relativePath);
		}
	}

	return path::absolute(proto_string);
}

instance<Module> FileLoader::load(instance<Namespace> ns, std::string const& proto_string, instance<> extra)
{
	auto ret = instance<FileLoader>::makeThroughLambda([](auto p) { return new (p) FileLoader(); });
	ret->_filePath = path::normalize(path::absolute(proto_string));

	return ret->_module = instance<Module>::make(ns, ret);
}

instance<> FileLoader::getContent()
{
	if (!path::exists(_filePath))
		throw stdext::exception("Module path `{0}` does not exist.", _filePath);

	auto text = craft::fs::read<std::string>(_filePath, &craft::fs::string_read).get();
	return CultLispSyntax::craft_s_typeDesc().getFeature<PSyntax>()->parse(text, _module, nullptr);
}
std::string FileLoader::getUri()
{
	return fmt::format("{0}:{1}", c_provider_index, _filePath);
}
bool FileLoader::prepSemantics(instance<>)
{
	return false;
}



CRAFT_DEFINE(ReplLoader)
{
	_.use<PModuleLoader>().singleton<AutoModuleLoader>();

	_.defaults();
}

std::string ReplLoader::resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra)
{
	return proto_string;
}

instance<Module> ReplLoader::load(instance<Namespace> ns, std::string const& proto_string, instance<> extra)
{
	auto ret = instance<ReplLoader>::makeThroughLambda([](auto p) { return new (p) ReplLoader(); });
	ret->_replName = proto_string;

	return ret->_module = instance<Module>::make(ns, ret);
}

instance<> ReplLoader::getContent()
{
	// TODO collect anon modules here
	return CultLispSyntax::craft_s_typeDesc().getFeature<PSyntax>()->parse("", _module, nullptr);
}
std::string ReplLoader::getUri()
{
	return fmt::format("{0}:{1}", c_provider_index, _replName);
}
bool ReplLoader::prepSemantics(instance<>)
{
	return false;
}



CRAFT_DEFINE(AnonLoader)
{
	_.use<PModuleLoader>().singleton<AutoModuleLoader>();

	_.defaults();
}

std::string AnonLoader::resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra)
{
	// TODO move anon counting here
	return proto_string;
}

instance<Module> AnonLoader::load(instance<Namespace> ns, std::string const& proto_string, instance<> extra)
{
	if (!extra || !extra.isType<AnonLoader>())
		throw stdext::exception("Anon loaders must be prebuilt.");

	auto ret = extra.asType<AnonLoader>();
	ret->_anonName = proto_string;

	return ret->_module = instance<Module>::make(ns, extra);
}

void AnonLoader::setModule(instance<Module> module)
{
	this->_asModule = module;
}
void AnonLoader::setContent(instance<> content)
{
	if (!content.isType<std::string>())
		throw stdext::exception("Anon loader content must be a string.");

	this->_content = content;
}

instance<> AnonLoader::getContent()
{
	return CultLispSyntax::craft_s_typeDesc().getFeature<PSyntax>()->parse(*_content.asType<std::string>(), _module, nullptr);
}
std::string AnonLoader::getUri()
{
	return fmt::format("{0}:{1}", c_provider_index, _anonName);
}
bool AnonLoader::prepSemantics(instance<> semantics)
{
	if (semantics.isType<CultSemantics>())
	{
		auto sem = semantics.asType<CultSemantics>();

		sem->readPrepDefaults();
		if(_asModule) sem->importModule(_asModule);

		return true;
	}

	return false;
}