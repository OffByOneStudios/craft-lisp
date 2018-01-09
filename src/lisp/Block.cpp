#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Block.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Block)
{
	_.use<SScope>().byCasting();

	_.defaults();
}

Block::Block(instance<SScope> parent)
{
	_environment = parent->environment();
	_parent = parent;
}


instance<Environment> Block::environment() const
{
	return _environment;
}
instance<Namespace> Block::namespace_() const
{
	return _parent->namespace_();
}
instance<SScope> Block::parent() const
{
	return _parent;
}

instance<SBinding> Block::lookup(std::string const& s)
{
	auto it = _lookup.find(s);
	if (it == _lookup.end())
	{
		if (_parent)
		{
			auto res = _parent->lookup(s);
			if (res.typeId().isType<Binding>())
			{
				return res;
			}
			else if (res.typeId().isType<BlockBinding>())
			{
				// return new ClosureBinding
				return instance<ClosureBinding>::make(res.asType<BlockBinding>(), 1);
			}
			else if (res.typeId().isType<ClosureBinding>())
			{
				// increment closure binding index
				res.asType<ClosureBinding>()->incrementClosure();
				return res;
			}
			else
			{
				throw stdext::exception("Block lookup, doesn't know if/how to close over binding {0}.", res.toString());
			}
		}
		else
			throw stdext::exception("Lookup failed, `{0}` not in scope.", s);
	}

	return _bindings[it->second];
}

instance<SBinding> Block::define(std::string name, instance<> value)
{
	auto binding = instance<BlockBinding>::make(_bindings.size(), name, value);
	_bindings.push_back(binding);

	_lookup[binding->name()] = binding->position();
	return binding;
}
