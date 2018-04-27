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

instance<Module> BuiltinLoader::load(instance<Namespace> ns, std::string const& proto_string, instance<> extra)
{
	auto ret = instance<BuiltinLoader>::makeThroughLambda([](auto p) { return new (p) BuiltinLoader(); });
	ret->_builtinName = proto_string;

	if (proto_string == "cult.system")
		return ret->_module = library::make_module_builtin_cult_system(ns, ret);
	else if (proto_string == "cult.core")
		return ret->_module = library::make_module_builtin_cult_core(ns, ret);
	else
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



CRAFT_DEFINE(FileLoader)
{
	_.use<PModuleLoader>().singleton<AutoModuleLoader>();

	_.defaults();
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
	return true;
}



CRAFT_DEFINE(ReplLoader)
{
	_.use<PModuleLoader>().singleton<AutoModuleLoader>();

	_.defaults();
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
		sem->importModule(_asModule);

		return true;
	}

	return false;
}