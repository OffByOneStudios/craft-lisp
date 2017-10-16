#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{
	enum class TypeKind
	{
		None = 0,

		//
		// Storage Type
		//

		// A vector of kinds
		Flag_Value = 1 << 8,
		Flag_Vector = 1 << 9,

		//
		// Type Kind Category
		//

		// A tuple of children
		Flag_Tuple = 1 << 4,
		Flag_Union = 1 << 5,

		//
		// Actuals
		//

		Any = Flag_Value + 0,
		Type = Flag_Value + 1,
		Abstract = Flag_Value + 2,

		Tuple = Flag_Tuple + Flag_Vector + 1,
		Union = Flag_Union + Flag_Vector + 1,
	};

	struct TypeStruct
	{

	};

	struct GenericTypeStruct
	{

	};
}}
