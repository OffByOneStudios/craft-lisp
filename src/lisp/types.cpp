#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/types.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::types;

CRAFT_ASPECT_DEFINE(SType);

CRAFT_OBJECT_DEFINE(Special)
{
	_.use<SType>().byCasting();

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
	_.use<SType>().byCasting();

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<CraftType> i) -> std::string
		{
			return i->type.toString(true);
		});

	_.defaults();
}
CRAFT_OBJECT_DEFINE(AbstractTag)
{
	_.use<SType>().byCasting();

	_.defaults();
}

CRAFT_OBJECT_DEFINE(Union)
{
	_.use<SType>().byCasting();

	_.defaults();
}
CRAFT_OBJECT_DEFINE(Tuple)
{
	_.use<SType>().byCasting();

	_.defaults();
}

CRAFT_OBJECT_DEFINE(TypeVar)
{
	_.use<SType>().byCasting();

	_.defaults();
}
CRAFT_OBJECT_DEFINE(Exists)
{
	_.use<SType>().byCasting();

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
	Type Special
*/

bool Special::isConcrete() const
{
	return false;
}
bool Special::isSubtypingSimple() const
{
	return true;
}

bool Special::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{
	if (this->kind == Any) return true;
	if (left.typeId().isType<Special>())
	{
		if (Special::isAny(left)) return false;
		if (Special::isBottom(left)) return true;
	}
	if (this->kind == Bottom) return false;

	assert(false && "");
	return false;
}


/*
	Type CraftType
*/

bool CraftType::isConcrete() const
{
	return true;
}
bool CraftType::isSubtypingSimple() const
{
	return true;
}

bool CraftType::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{
	if (Special::isBottom(left)) return true;

	if (left.typeId().isType<CraftType>())
	{
		return this->type == left.asType<CraftType>()->type;
	}

	assert(false && "");
	return false;
}


/*
Type AbstractTag
*/

bool AbstractTag::isConcrete() const
{
	return false;
}
bool AbstractTag::isSubtypingSimple() const
{
	return true;
}

bool AbstractTag::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{
	if (Special::isBottom(left)) return true;

	if (left.typeId().isType<CraftType>())
	{
		return env->type_isChild(left.asType<CraftType>()->type, tag);
	}
	else if (left.typeId().isType<AbstractTag>())
	{
		return env->type_isChild(left.asType<AbstractTag>()->tag, tag);
	}

	assert(false && "");
	return false;
}


/*
	Type Union
*/

bool Union::isConcrete() const
{
	return false;
}
bool Union::isSubtypingSimple() const
{
	return false;
}

bool Union::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{

	assert(false && "");
	return false;
}


/*
	Type Tuple
*/

bool Tuple::isConcrete() const
{
	return false;
}
bool Tuple::isSubtypingSimple() const
{
	return false;
}

bool Tuple::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{

	assert(false && "");
	return false;
}


/*
	Type TypeVar
*/

bool TypeVar::isConcrete() const
{
	return false;
}
bool TypeVar::isSubtypingSimple() const
{
	return false;
}

bool TypeVar::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{

	assert(false && "");
	return false;
}


/*
	Type Exists
*/

bool Exists::isConcrete() const
{
	return false;
}
bool Exists::isSubtypingSimple() const
{
	return false;
}

bool Exists::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{

	assert(false && "");
	return false;
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
		return;
	}

	// Ok prepare algorithm...
	assert(false);

	// We finished yay!
	executed = true;
}

int AlgorithmSubtype::_trivialSubtype(instance<Environment> env, instance<SType> left, instance<SType> right)
{
	// TODO:
	// It should be possible to make types into an interface for this function
	// * right->isSubtype(Environment, left);

	//
	// Specials
	//
	if (Special::isAny(right)) return 1;
	if (Special::isAny(left)) return -1;
	if (Special::isBottom(left)) return 1;
	if (Special::isBottom(right)) return -1;

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

		size_t trivialTrue = 0;
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

	if (!left->isSubtypingSimple() || !right->isSubtypingSimple())
		return 0;

	return right->isSubtype(env, left, nullptr);
}
