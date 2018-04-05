
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_meta_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto getmeta = instance<Macro>::make(
		[](instance<SScope> scope, std::vector<instance<>> const& code)
	{
		instance<Symbol> a(expect<Symbol>(code[1]));
		instance<std::string> b(expect<std::string>(code[2]));


		auto t = scope->namespace_()->lookup(a->value);
		return t->getMeta(*b);
	});
	ret->define_eval("getmeta", getmeta);
	
	auto setmeta = instance<Macro>::make(
		[](instance<SScope> scope, std::vector<instance<>> const& code)
	{
		instance<Symbol> a(expect<Symbol>(code[1]));
		instance<std::string> b(expect<std::string>(code[2]));

		auto t = scope->namespace_()->lookup(a->value);

		auto res = t->getMeta(*b);
		t->addMeta(*b, code[3]);

		return res;
	});
	ret->define_eval("setmeta", setmeta);
	
}
