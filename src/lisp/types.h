#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{
	namespace types
	{
		using namespace craft::types;

		struct TagId final
			: stdext::IdValue<TagId, uint64_t>
		{
			using IdValue::IdValue;
		};

		class Special
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Special);
		public:
			enum {
				Any = 0,
				Bottom = 255
			} kind;

			inline static bool isAny(instance<> const& i) { return i.typeId().isType<Special>() && i.asType<Special>()->kind == Any; }
			inline static bool isBottom(instance<> const& i) { return i.typeId().isType<Special>() && i.asType<Special>()->kind == Bottom; }
		};

		class CraftType
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::CraftType);
		public:
			TypeId type;

			inline CraftType(TypeId id) : type(id) { }
		};

		class AbstractTag
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::AbstractTag);
		public:
			TagId tag;

			inline AbstractTag(TagId id) : tag(id) { }
		};

		class Union
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Union);
		public:
			std::vector<instance<>> variants;
		};

		class Tuple
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Tuple);
		public:
			std::vector<instance<>> cells;
		};

		class TypeVar
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::TypeVar);
		public:
			std::string symbol;
		};

		class Exists
			: public virtual craft::types::Object
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Exists);
		public:
			instance<TypeVar> typeVar;
			instance<> subExpr;
		};

		/*
			The subtype relationship between `left <: right` where left is a subtype of right (e.g. it is more specific).

			Axioms:
			1. T <: Any
			2. Bottom <: T
			3. T <: T
			4. T <: U[A_0, ..., A_N]
			  =>   E   :  T <: A_0
			5. Tuple[A_0, ..., A_N] <: Tuple[B_0, ..., B_N]
			  =>   \-/ :  I | A_I <: B_I

		*/
		class AlgorithmSubtype
		{
		private: // init
			instance<Environment> _env;
			instance<> _left;
			instance<> _right;

		private: // helpers

			// 1 is true, 0 is undecided, -1 is false
			static int _trivialSubtype(instance<Environment> env, instance<> left, instance<> right);

		private: // state
			struct UnionState
			{
				size_t depth;
				bool more;
				std::vector<size_t> choices;
			};

			struct TypeVarState
			{
				instance<> lowerBound;
				instance<> upperBound;
				bool rightSide;
			};

			std::map<instance<TypeVar>, TypeVarState> _typeVars;
			UnionState _leftStack;
			UnionState _rightStack;

		public: // result:
			bool executed;
			bool leftIsSubtype;

		public:
			CRAFT_LISP_EXPORTED AlgorithmSubtype(instance<Environment> env, instance<> left, instance<> right);

			CRAFT_LISP_EXPORTED void execute();
		};
	}

	/* Makes a type expression that represents a sequence of live vars
	*/
	CRAFT_LISP_EXPORTED instance<> type_of(instance<Environment> env, std::vector<instance<>> const& args);
}}
