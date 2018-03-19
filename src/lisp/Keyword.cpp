#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Keyword.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Keyword)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Keyword> i) { return ":" + i->value; });

	_.defaults();
}

Keyword::Keyword(std::string const& s)
{
	assert(s.size() > 1 && s[0] == ':');
	value = s.substr(1);
}
