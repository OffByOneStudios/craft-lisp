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

		std::unique_ptr<llvm::TargetMachine> _tm;
		const llvm::DataLayout _dl;
		llvm::orc::RTDyldObjectLinkingLayer _objectLayer;
		llvm::orc::IRCompileLayer<decltype(_objectLayer), llvm::orc::SimpleCompiler> _compileLayer;

	public:
		using ModuleHandle = decltype(_compileLayer)::ModuleHandleT;

	public:

		CRAFT_LISP_EXPORTED LlvmBackend();

	public:

		CRAFT_LISP_EXPORTED ModuleHandle addModule(std::unique_ptr<llvm::Module> module);

		CRAFT_LISP_EXPORTED llvm::JITSymbol findSymbol(std::string const& name);
		CRAFT_LISP_EXPORTED llvm::JITTargetAddress getSymbolAddress(std::string const& name);

		CRAFT_LISP_EXPORTED void removeModule(ModuleHandle module);
	};

	class LlvmBackendProvider sealed
		: public types::Implements<PBackend>::For<LlvmBackend>
	{
	public:
		CRAFT_LISP_EXPORTED LlvmBackendProvider();

	public:
		virtual void init(instance<Environment> env) const;
	};

}}