#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/Keyword.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_DEFINE(Keyword)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Keyword> i) { return ":" + i->Resolve(); });

	_.defaults();
}

std::string const& Keyword::Resolve() const
{
	return Execution::getCurrent()->getNamespace()->symbolStore.Resolve(symbolStoreId);
}

instance<Keyword> Keyword::makeKeyword(std::string const& s)
{
	std::string value(s);

	if (s.size() > 0 && s[0] == ':')
		value = s.substr(1);

	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;

	auto nsym = instance<Keyword>::makeFromPointer(new Keyword());
	nsym->symbolStoreId = symbol_store.intern(s);
	return nsym;

}