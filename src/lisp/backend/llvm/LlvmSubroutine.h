#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/llvm/llvm_internal.h"

namespace craft {
namespace lisp
{
	class LlvmSubroutine
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::LlvmSubroutine);
	public:
		instance<LlvmModule> _module;

		std::string _binding_hint;
		instance<> _lisp;

		llvm::Function* func;


		CRAFT_LISP_EXPORTED llvm::FunctionType* getLlvmType(instance<LlvmBackend> backend, instance<SubroutineSignature> signature);

	public:

		CRAFT_LISP_EXPORTED LlvmSubroutine(instance<LlvmModule>, instance<>);

	public:

		CRAFT_LISP_EXPORTED void generate();

	};
}}
