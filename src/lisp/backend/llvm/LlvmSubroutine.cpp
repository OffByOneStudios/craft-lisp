#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/llvm/llvm_internal.h"
#include "lisp/backend/llvm/LlvmSubroutine.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

using namespace llvm;
using namespace llvm::orc;

CRAFT_OBJECT_DEFINE(LlvmSubroutine)
{
	_.defaults();
}

LlvmSubroutine::LlvmSubroutine(instance<LlvmModule> module, instance<> lisp)
{
	_parent = module;
	_lisp = lisp;
}

void LlvmSubroutine::generate()
{
	auto type = getLlvmType(_parent->_parent, _lisp.asFeature<PSubroutine>()->signature(_lisp));

	func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "test", _parent->ir.get());
}

llvm::FunctionType* LlvmSubroutine::getLlvmType(instance<LlvmBackend> backend, instance<SubroutineSignature> signature)
{
	llvm::Type* return_;
	std::vector<llvm::Type*> args;

	for (auto arg : signature->arguments)
	{
		args.push_back(backend->_type_instance);
	}

	return_ = backend->_type_instance;

	return llvm::FunctionType::get(return_, args, false);
}
