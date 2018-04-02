#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/llvm/llvm_internal.h"

namespace craft {
namespace lisp
{
	class LlvmBackend
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::LlvmBackend);
	private:
		friend class LlvmBackendProvider;

		friend class LlvmModule;
		friend class LlvmSubroutine;

		llvm::LLVMContext _context;

		std::unique_ptr<llvm::TargetMachine> _tm;
		const llvm::DataLayout _dl;
		llvm::orc::RTDyldObjectLinkingLayer _objectLayer;
		llvm::orc::IRCompileLayer<decltype(_objectLayer), llvm::orc::SimpleCompiler> _compileLayer;

		std::shared_ptr<llvm::JITSymbolResolver> _resolver;

		llvm::Type* _type_anyPtr;
		llvm::Type* _type_instance;

	public:
		instance<Namespace> lisp;

	public:
		using ModuleHandle = decltype(_compileLayer)::ModuleHandleT;

	public:

		CRAFT_LISP_EXPORTED LlvmBackend(instance<Namespace>);

	public:

		CRAFT_LISP_EXPORTED void addModule(instance<LlvmModule> module);

		CRAFT_LISP_EXPORTED llvm::JITSymbol findSymbol(std::string const& name);
		CRAFT_LISP_EXPORTED llvm::JITTargetAddress getSymbolAddress(std::string const& name);

		CRAFT_LISP_EXPORTED void removeModule(ModuleHandle module);
	};

	class LlvmBackendProvider final
		: public types::Implements<PBackend>::For<LlvmBackend>
	{
	public:
		CRAFT_LISP_EXPORTED LlvmBackendProvider();

	public:
		virtual instance<> init(instance<Namespace> env) const override;

		virtual instance<> addModule(instance<> backend_ns, instance<Module>) const override;
		virtual instance<> addFunction(instance<> backend_module, instance<>) const override;

		virtual instance<> exec(instance<lisp::SFrame> frame, instance<> code) const override;
	};

}}
