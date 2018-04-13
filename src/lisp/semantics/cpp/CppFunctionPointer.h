#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cpp/cpp_semantics.h"

namespace craft {
namespace lisp
{
	class CppFunctionPointerSubroutineProvider;

	class CppFunctionPointer
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CppFunctionPointer)
	private:
		friend class CppFunctionPointerSubroutineProvider;

		types::ExpressionStore expression;
		types::Function function;

	public:
		CRAFT_LISP_EXPORTED CppFunctionPointer(types::ExpressionStore const&, types::Function const&);
	};
}}
