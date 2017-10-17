#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/types.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::types;

CRAFT_OBJECT_DEFINE(Special)
{
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<Special> i) -> std::string
		{
			if (Special::isAny(i)) return "Any";
			if (Special::isBottom(i)) return "Bottom";
			return "SpecialType.INVALID";
		});

	_.defaults();
}
CRAFT_OBJECT_DEFINE(CraftType)
{
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<CraftType> i) -> std::string
		{
			return i->type.toString(true);
		});

	_.defaults();
}
CRAFT_OBJECT_DEFINE(AbstractTag)
{
	_.defaults();
}

CRAFT_OBJECT_DEFINE(Union)
{
	_.defaults();
}
CRAFT_OBJECT_DEFINE(Tuple)
{
	_.defaults();
}

CRAFT_OBJECT_DEFINE(TypeVar)
{
	_.defaults();
}
CRAFT_OBJECT_DEFINE(Exists)
{
	_.defaults();
}

instance<> lisp::type_of(instance<Environment> env, std::vector<instance<>> const& args)
{
	auto ret = instance<Tuple>::make();

	ret->cells.reserve(args.size());
	for (auto a : args)
		ret->cells.push_back(instance<CraftType>::make(a.typeId()));

	return ret;
}

/*
Subtype
*/

AlgorithmSubtype::AlgorithmSubtype(instance<Environment> env, instance<> left, instance<> right)
{
	_env = env;
	_left = left;
	_right = right;

	executed = false;
}

void AlgorithmSubtype::execute()
{
	// Trivial checks first
	auto trivial = _trivialSubtype(_env, _left, _right);

	if (trivial != 0)
	{
		executed = true;
		leftIsSubtype = trivial > 0;
	}

	// Ok prepare algorithm...

	// We finished yay!
	executed = true;
}

int AlgorithmSubtype::_trivialSubtype(instance<Environment> env, instance<> left, instance<> right)
{
	// TODO:
	// It should be possible to make types into an interface for this function
	// * isSimpleType(); // this function is undecideable if either is false (sans any / bottom).
	// * right->isSubtype(Environment, left);

	//
	// Specials
	//
	if (Special::isAny(right)) return 1;
	if (Special::isAny(left)) return -1;
	if (Special::isBottom(left)) return 1;
	if (Special::isBottom(right)) return -1;

	// Unions, type vars, and related requires stack
	if (left.typeId().isType<Union>()) return 0;
	if (right.typeId().isType<Union>()) return 0;
	if (left.typeId().isType<TypeVar>()) return 0;
	if (right.typeId().isType<TypeVar>()) return 0;
	if (left.typeId().isType<Exists>()) return 0;
	if (right.typeId().isType<Exists>()) return 0;

	//
	// Concrete Types
	//
	auto leftIsType = left.typeId().isType<CraftType>();
	auto rightIsType = right.typeId().isType<CraftType>();

	// 1 parametric combo (type)
	if (leftIsType && rightIsType)
	{
		if (left.asType<CraftType>()->type == right.asType<CraftType>()->type)
			return 1;
		else
			return -1;
	}

	//
	// Tags
	//
	auto leftIsTag = left.typeId().isType<AbstractTag>();
	auto rightIsTag = right.typeId().isType<AbstractTag>();

	// 3 parametric combos (tag * type - (type))
	if (rightIsTag)
	{ // 2
		if (leftIsTag)
			return env->type_isChild(left.asType<AbstractTag>()->tag, right.asType<AbstractTag>()->tag) ? 1 : -1;
		if (leftIsType)
			return env->type_isChild(left.asType<CraftType>()->type, right.asType<AbstractTag>()->tag) ? 1 : -1;
	}
	if (leftIsTag)
	{ // 1
		if (rightIsType)
			return -1;
	}

	//
	// Tuples
	//
	if (left.typeId().isType<Tuple>())
	{
		if (!right.typeId().isType<Tuple>())
			return -1; // Both must be tuples if either are (excluding unions)

		auto leftTuple = left.asType<Tuple>();
		auto rightTuple = right.asType<Tuple>();

		auto leftSize = leftTuple->cells.size();

		if (leftSize != rightTuple->cells.size())
			return -1;

		int trivialTrue = 0;
		for (int i = 0; i < leftSize; ++i)
		{
			auto trivial = _trivialSubtype(env, leftTuple->cells[i], rightTuple->cells[i]);
			if (trivial == 0) continue;
			else if (trivial > 0) trivialTrue += 1;
			else if (trivial < 0) return -1; // Any false pair ruins it all
		}

		if (trivialTrue = leftSize)
			return 1; // True if all pairs are true
	}
	else if (right.typeId().isType<Tuple>())
		return -1;  // Both must be tuples if either are (excluding unions)

	return 0;
}
