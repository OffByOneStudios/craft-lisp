#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/Keyword.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Keyword)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Keyword> i) { return ":" + i->getValue(); });

	_.defaults();
}

std::string const& Keyword::getValue() const
{
	return Execution::getCurrent()->getNamespace()->symbolStore.getValue(symbolStoreId);
}

instance<Keyword> Keyword::makeKeyword(std::string const& s)
{
	std::string value(s);

	if (s.size() > 0 && s[0] == ':')
		value = s.substr(1);

	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;

	auto nsym = new Keyword();
	nsym->craft_setupInstance();

	nsym->symbolStoreId = symbol_store.intern(s);

	return instance<Keyword>(nsym);

}