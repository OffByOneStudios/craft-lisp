#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/Symbol.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(Symbol)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Symbol> i) { return i->getValue(); });

	_.defaults();
}

std::string const& Symbol::getValue() const
{
	Execution::getCurrent()->getNamespace()->symbolStore.getValue(symbolStoreId);
}

instance<Symbol> Symbol::makeSymbol(instance<Keyword> keyword)
{
	auto nsym = new Symbol();
	nsym->craft_setupInstance();

	nsym->symbolStoreId = keyword->symbolStoreId;

	return instance<Symbol>(nsym);
}
instance<Symbol> Symbol::makeSymbol(std::string const& s)
{
	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;

	auto nsym = new Symbol();
	nsym->craft_setupInstance();
	
	nsym->symbolStoreId = symbol_store.intern(s);
	
	return instance<Symbol>(nsym);
}
