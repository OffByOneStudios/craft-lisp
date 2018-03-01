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
CRAFT_OBJECT_DEFINE(CraftFeature)
{
	_.use<SType>().byCasting();

	_.use<PStringer>().singleton<FunctionalStringer>(
		[](instance<CraftFeature> i) -> std::string
	{
		return i->feature.toString(true);
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

bool SType::reinvokeSubtype(AlgorithmSubtype* algo, instance<Environment> env, instance<> left, instance<> right)
{
	if (algo == nullptr)
	{
		return AlgorithmSubtype::_trivialSubtype(env, left, right);
	}
	else
	{
		return algo->reinvoke(left, right);
	}
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
	Type CraftType
*/

bool CraftFeature::isConcrete() const
{
	return false;
}
bool CraftFeature::isSubtypingSimple() const
{
	return true;
}

bool CraftFeature::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{
	if (Special::isBottom(left)) return true;

	if (left.typeId().isType<CraftFeature>())
	{
		return this->feature == left.asType<CraftFeature>()->feature;
	}
	if (left.typeId().isType<CraftType>())
	{
		return system().typeHasFeature(left.asType<CraftType>()->type, feature);
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
	Type Tuple
*/

bool Tuple::isConcrete() const
{
	return false;
}
bool Tuple::isSubtypingSimple() const
{
	return std::all_of(cells.begin(), cells.end(), [](instance<SType> i) -> bool { return i->isSubtypingSimple(); });
}

bool Tuple::isSubtype(instance<Environment> env, instance<> left, AlgorithmSubtype* algo) const
{
	if (left.typeId().isType<Tuple>())
	{
		auto leftTuple = left.asType<Tuple>();
		auto leftSize = leftTuple->cells.size();

		if (leftSize != cells.size())
			return false;

		for (int i = 0; i < leftSize; ++i)
		{
			if (!reinvokeSubtype(algo, env, leftTuple->cells[i], cells[i]))
				return false;
		}

		return true;
	}
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

// This algorithm could be refactored into a stack of where it is in each tree
// And then a simple double dispatch mechanism to compare types

AlgorithmSubtype::AlgorithmSubtype(instance<Environment> env, instance<> left, instance<> right)
{
	_env = env;
	_left = left;
	_right = right;

	executed = false;
}

bool AlgorithmSubtype::_isSimple(instance<Environment> env, instance<SType> left, instance<SType> right)
{
	return left->isSubtypingSimple() && right->isSubtypingSimple();
}
bool AlgorithmSubtype::_trivialSubtype(instance<Environment> env, instance<SType> left, instance<SType> right, AlgorithmSubtype* algo)
{
	//
	// Specials
	//
	if (Special::isAny(right)) return true;
	if (Special::isAny(left)) return false;
	if (Special::isBottom(left)) return true;
	if (Special::isBottom(right)) return false;

	//
	// Core typing
	//
	return right->isSubtype(env, left, algo);
}

instance<> AlgorithmSubtype::_pickUnionType(instance<Union> union_, UnionState& state)
{
	if (!(state.depth < state.choices.size()))
	{
		// If we haven't gotten this deep before push it to the state stack.
		state.choices.push_back(union_->variants.size() - 1);
	}

	auto choice = state.choices[state.depth];
	state.depth++;

	return union_->variants[choice];
}

bool AlgorithmSubtype::reinvoke(instance<SType> newLeft, instance<SType> newRight)
{
	if (newLeft.typeId().isType<Union>())
	{
		newLeft = _pickUnionType(newLeft.asType<Union>(), _leftStack);

		return reinvoke(newLeft, newRight);
	}
	else if (newRight.typeId().isType<Union>())
	{
		newRight = _pickUnionType(newRight.asType<Union>(), _rightStack);

		return reinvoke(newLeft, newRight);
	}
	else
	{
		return _trivialSubtype(_env, newLeft, newRight, this);
	}
}

void AlgorithmSubtype::execute_subtype()
{
	// Trivial checks first
	if (_isSimple(_env, _left, _right))
	{
		executed = true;
		leftIsSubtype = _trivialSubtype(_env, _left, _right);
		return;
	}

	// Ok prepare algorithm...
	_typeVars.clear();

	// For each left hand union choice there must be a matching right hand that accepts.
	// If there is none then it's a failure.
	// For right hand choices we we search until we find what we are looking for.
	// I suspect the existential queries will require this behviour...

	// This function will be both loops.

	// And do it
	_leftStack.choices.clear();
	do
	{ // Outer forall loop that checks each left hand side for acceptance
		bool found = false;
		_rightStack.choices.clear();

		do
		{ // Inner exists loop that checks any right hand side for existence
			_leftStack.depth = 0;
			_rightStack.depth = 0;

			// check for breaks
			if (reinvoke(_left, _right))
			{
				found = true;
				break;
			}

			_rightStack.incrementChoice();

		} while (!_rightStack.choices.empty());

		if (!found)
		{
			executed = true;
			leftIsSubtype = false;
			return;
		}

		_leftStack.incrementChoice();
	}
	while (!_leftStack.choices.empty());

	// We finished yay!
	executed = true;
	leftIsSubtype = true;
}

bool lisp::types::is_subtype(instance<Environment> env, instance<SType> left, instance<SType> right)
{
	lisp::types::AlgorithmSubtype subtype(env, left, right);
	subtype.execute_subtype();
	return subtype.leftIsSubtype;
}