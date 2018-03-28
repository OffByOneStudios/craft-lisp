#pragma once

#include "lisp/common.h"

//
// LLVM
//

#ifdef _MSC_VER
#pragma warning( push, 1 )
#pragma warning( disable : 4141 )
#pragma warning( disable : 4291 )
#pragma warning( disable : 4624 )
#endif

#include "llvm/Support/TargetSelect.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/RTDyldMemoryManager.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/LambdaResolver.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

#ifdef _MSC_VER
#pragma warning( pop )
#endif

//
// Ours
//

#include "lisp/backend/backend.h"
#include "lisp/backend/BootstrapInterpreter.h"

#include "lisp/semantics/cult/cult_semantics.h"

//
// Lisp Object
//

namespace craft {
namespace lisp
{
	class LlvmBackend;
	class LlvmModule;
	class LlvmSubroutine;
}}

#include "LlvmCompiler.h"
#include "LlvmBackend.h"
#include "LlvmModule.h"
#include "LlvmSubroutine.h"
