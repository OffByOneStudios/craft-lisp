#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/llvm/llvm_internal.h"
#include "lisp/backend/llvm/LlvmBackend.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

using namespace llvm;
using namespace llvm::orc;

CRAFT_OBJECT_DEFINE(LlvmBackend)
{
	_.use<PBackend>().singleton<LlvmBackendProvider>();

	_.defaults();
}

LlvmBackend::LlvmBackend()
	: _tm(EngineBuilder().selectTarget()) // from current process
	, _dl(_tm->createDataLayout())
	, _objectLayer([]() { return std::make_shared<SectionMemoryManager>(); }) // lambda to make memory sections
	, _compileLayer(_objectLayer, SimpleCompiler(*_tm))
{
	llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr); // load the current process
}

LlvmBackend::ModuleHandle LlvmBackend::addModule(std::unique_ptr<llvm::Module> M)
{
	// Build our symbol resolver:
	// Lambda 1: Look back into the JIT itself to find symbols that are part of
	//           the same "logical dylib".
	// Lambda 2: Search for external symbols in the host process.
	auto Resolver = createLambdaResolver(
		[&](const std::string &Name) {
		if (auto Sym = _compileLayer.findSymbol(Name, false))
			return Sym;
		return JITSymbol(nullptr);
	},
		[](const std::string &Name) {
		if (auto SymAddr =
			RTDyldMemoryManager::getSymbolAddressInProcess(Name))
			return JITSymbol(SymAddr, JITSymbolFlags::Exported);
		return JITSymbol(nullptr);
	});

	// Add the set to the JIT with the resolver we created above and a newly
	// created SectionMemoryManager.
	return cantFail(_compileLayer.addModule(std::move(M),
		std::move(Resolver)));
}

JITSymbol LlvmBackend::findSymbol(std::string const& name)
{
	std::string mangled_name;
	raw_string_ostream mangled_name_stream(mangled_name);
	Mangler::getNameWithPrefix(mangled_name_stream, name, _dl);
	return _compileLayer.findSymbol(mangled_name_stream.str(), true);
}

JITTargetAddress LlvmBackend::getSymbolAddress(std::string const& name)
{
	return cantFail(findSymbol(name).getAddress());
}

void LlvmBackend::removeModule(ModuleHandle H)
{
	cantFail(_compileLayer.removeModule(H));
}

LlvmBackendProvider::LlvmBackendProvider()
{
	llvm::InitializeNativeTarget();
}

void LlvmBackendProvider::init(instance<Environment> env) const
{
	env->backend = instance<LlvmBackend>::make();
}
