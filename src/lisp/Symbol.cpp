#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Symbol.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Symbol)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Symbol> i) { return i->value; });

	_.defaults();
}

Symbol::Symbol(std::string const& s)
{
	value = s;
}