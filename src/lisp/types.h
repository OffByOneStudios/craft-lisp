#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp
{
	namespace types
	{
		using namespace craft::types;

		class AlgorithmSubtype;

		struct TagId final
			: stdext::IdValue<TagId, uint64_t>
		{
			using IdValue::IdValue;
		};

		class SType
			: public types::Aspect
		{
			CRAFT_LISP_EXPORTED CRAFT_ASPECT_DECLARE(craft::lisp::types::SType, "lisp.type", types::FactoryAspectManager);
		protected:

			CRAFT_LISP_EXPORTED static bool reinvokeSubtype(AlgorithmSubtype*, instance<Environment> env, instance<> left, instance<> right);

		public:

			virtual bool isConcrete() const = 0;
			virtual bool isSubtypingSimple() const = 0;

			virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const = 0;
		};

		class Special
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Special);
		public:
			enum {
				Any = 0,
				Bottom = 255
			} kind;

			inline static bool isAny(instance<> const& i) { return i.typeId().isType<Special>() && i.asType<Special>()->kind == Any; }
			inline static bool isBottom(instance<> const& i) { return i.typeId().isType<Special>() && i.asType<Special>()->kind == Bottom; }

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class CraftType
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::CraftType);
		public:
			TypeId type;

			inline CraftType(TypeId id) : type(id) { }

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class CraftFeature
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::CraftFeature);
		public:
			FeatureId feature;

			inline CraftFeature(FeatureId id) : feature(id) { }

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class AbstractTag
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::AbstractTag);
		public:
			TagId tag;

			inline AbstractTag(TagId id) : tag(id) { }

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class Union
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Union);
		public:
			std::vector<instance<>> variants;

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class Tuple
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Tuple);
		public:
			std::vector<instance<>> cells;

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class TypeVar
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::TypeVar);
		public:
			std::string symbol;

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
		};

		class Exists
			: public virtual craft::types::Object
			, Implements<SType>
		{
			CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::types::Exists);
		public:
			// This typevar must be the referential equivelent of the one inside the expression
			instance<TypeVar> typeVar;

			// The body type expression
			instance<> subExpr;

		public:
			CRAFT_LISP_EXPORTED virtual bool isConcrete() const override;
			CRAFT_LISP_EXPORTED virtual bool isSubtypingSimple() const override;

			CRAFT_LISP_EXPORTED virtual bool isSubtype(instance<Environment>, instance<> left, AlgorithmSubtype* = nullptr) const override;
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

		protected:
			friend class SType;

			CRAFT_LISP_EXPORTED bool reinvoke(instance<SType> newLeft, instance<SType> newRight);

		private: // state
			struct UnionState
			{
				size_t depth;
				size_t more;
				std::vector<size_t> choices;

				inline void incrementChoice()
				{
					while (!choices.empty() && choices.back() == 0)
						choices.pop_back();
					if (!choices.empty())
						choices[choices.size() - 1] -= 1;
				}
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

		private: // helpers

			static bool _isSimple(instance<Environment> env, instance<SType> left, instance<SType> right);
			static bool _trivialSubtype(instance<Environment> env, instance<SType> left, instance<SType> right, AlgorithmSubtype* algo = nullptr);
			static instance<> _pickUnionType(instance<Union> union_, UnionState& state);

		public: // result:
			bool executed;
			bool leftIsSubtype;

		public:
			CRAFT_LISP_EXPORTED AlgorithmSubtype(instance<Environment> env, instance<> left, instance<> right);

			CRAFT_LISP_EXPORTED void execute_subtype();
		};

		CRAFT_LISP_EXPORTED bool is_subtype(instance<Environment> env, instance<types::SType> left, instance<types::SType> right);
	}

	/* Makes a type expression that represents a sequence of live vars
	*/
	CRAFT_LISP_EXPORTED instance<> type_of(instance<Environment> env, std::vector<instance<>> const& args);
}}
