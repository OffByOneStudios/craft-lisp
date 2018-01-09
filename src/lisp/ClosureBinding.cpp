#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/ClosureBinding.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(ClosureBinding)
{
	_.use<SBinding>().byCasting();

	_.defaults();
}

ClosureBinding::ClosureBinding(instance<BlockBinding> base, size_t pos)
{
	_base = base;
	_lexical_pos = pos;
}

size_t ClosureBinding::positionLexical() const
{
	return _lexical_pos;
}
size_t ClosureBinding::positionFrame() const
{
	return _base->position();
}

void ClosureBinding::incrementClosure()
{
	_lexical_pos += 1;
}
void ClosureBinding::setClosure(size_t lexical_pos)
{
	_lexical_pos = lexical_pos;
}

std::string ClosureBinding::name() const
{
	return _base->name();
}
instance<> ClosureBinding::getValue(instance<SFrame> frame) const
{
	for (int i = _lexical_pos; i != 0; i--)
	{
		frame = frame->lexical_parent();
		if (!frame)
			throw stdext::exception("Lexical frames not populated.");
	}

	return _base->getValue(frame);
}

instance<> ClosureBinding::eval(instance<SFrame> frame)
{
	throw stdext::exception("Can't eval-initalize a closed over binding.");
}

instance<> ClosureBinding::getMeta(std::string metaKey, TypeId type)
{
	return _base->getMeta(metaKey, type);
}
