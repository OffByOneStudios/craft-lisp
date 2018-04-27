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
	return Execution::getCurrent()->getNamespace()->symbolStore.getValue(symbolStoreId);
}

instance<Symbol> Symbol::makeSymbol(instance<Keyword> keyword)
{
	auto nsym = instance<Symbol>::makeThroughLambda([](auto p) { return new (p) Symbol(); });
	nsym->symbolStoreId = keyword->symbolStoreId;

	return nsym;
}
instance<Symbol> Symbol::makeSymbol(std::string const& s)
{
	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;

	auto nsym = instance<Symbol>::makeThroughLambda([](auto p) { return new (p) Symbol(); });
	nsym->symbolStoreId = symbol_store.intern(s);
	
	return nsym;
}
