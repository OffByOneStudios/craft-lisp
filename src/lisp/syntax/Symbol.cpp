#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/Symbol.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(Symbol)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Symbol> i) { return i->Resolve(); });

	_.defaults();
}

std::string const& Symbol::Resolve() const
{
	return Execution::getCurrent()->getNamespace()->symbolStore.Resolve(symbolStoreId);
}

instance<Symbol> Symbol::makeSymbol(instance<Keyword> keyword)
{
	auto nsym = instance<Symbol>::makeFromPointer(new Symbol());
	nsym->symbolStoreId = keyword->symbolStoreId;

	return nsym;
}
instance<Symbol> Symbol::makeSymbol(std::string const& s)
{
	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;

	auto nsym = instance<Symbol>::makeFromPointer(new Symbol());
	nsym->symbolStoreId = symbol_store.intern(s);
	
	return nsym;
}
