
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void core::make_meta_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	//TODO Macro implementation on cult semantics
	/*semantics->builtin_addSpecialForm
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
	
	auto signatures = instance<MultiMethod>::make();
	signatures->attach(env, instance<BuiltinFunction>::make(
		[](auto frame, auto args)
	{
		instance<MultiMethod> a(expect<MultiMethod>(args[0]));

		auto res = instance<List>::make();
		
		for (auto it : a->signatures())
		{
			res->push(it);
		}
		return res;
	}));
	ret->define_eval("multimethod/signatures", signatures);*/
}
