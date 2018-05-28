#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/Symbol.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

CRAFT_DEFINE(Symbol)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Symbol> i) { return i->getDisplay(); });

	_.defaults();
}

size_t Symbol::size() const
{
	return _symbolseq.size();
}
uint32_t Symbol::at(size_t p) const
{
	return _symbolseq.at(p);
}
bool Symbol::isKeyword() const
{
	auto size = _symbolseq.size();
	if (size == 1) return false;
	else if (size == 2) return _symbolseq[0] == (uint32_t)Path::Colon && !isPath(_symbolseq[1]);
	else return false
		// :some.arbitrary/keyword
		|| _symbolseq[0] == (uint32_t)Path::Colon
		// some/arbitrary/:keyword
		|| (isPath(_symbolseq[size - 3]) && _symbolseq[size - 2] == (uint32_t)Path::Colon && !isPath(_symbolseq[size - 1]));
}
bool Symbol::isSimple() const
{
	return _symbolseq.size() == 1;
}

std::string Symbol::getDisplay() const
{
	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;
	auto size = _symbolseq.size();

	if (size == 1 && !isPath(_symbolseq[0]))
		return symbol_store.getValue(_symbolseq[0]);

	std::string res;

	for (auto s : _symbolseq)
	{
		if (isPath(s))
			res += toChar(s);
		else
			res += symbol_store.getValue(s);
	}

	return res;
}


instance<Symbol> Symbol::makeSymbol(std::string const& s)
{
	auto& symbol_store = Execution::getCurrent()->getNamespace()->symbolStore;

	auto const c_lookup_str = ":.@"; // "/:.@"
	auto const c_lookup_str_end = c_lookup_str + 3;
	const Path c_lookup_path[3] = { /*Path::Slash,*/ Path::Colon, Path::Dot, Path::At };

	auto size = s.size();
	auto nsym = instance<Symbol>::makeThroughLambda([](auto p) { return new (p) Symbol(); });
	auto last_i = 0;
	for (auto i = 0; i < size; ++i)
	{
		auto l = std::find(c_lookup_str, c_lookup_str_end, s[i]);
		if (l == c_lookup_str_end)
			continue;

		auto count = i - last_i;
		if (count != 0)
			nsym->_symbolseq.push_back(symbol_store.intern(s.substr(last_i, count)));

		auto pathsep = c_lookup_path[l - c_lookup_str];
		nsym->_symbolseq.push_back((uint32_t)pathsep);
		last_i = i + 1;
	}

	nsym->_symbolseq.push_back(symbol_store.intern(s.substr(last_i)));

	return nsym;
}

instance<Symbol> Symbol::trimKeyword() const
{
	if (!isKeyword()) throw stdext::exception("Not a keyword");
	auto nsym = instance<Symbol>::makeThroughLambda([](auto p) { return new (p) Symbol(); });

	nsym->_symbolseq.reserve(this->_symbolseq.size() - 1);
	std::copy(this->_symbolseq.begin() + 1, this->_symbolseq.end(), std::back_inserter(nsym->_symbolseq));
	return nsym;
}