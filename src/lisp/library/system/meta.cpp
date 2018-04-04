
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

	auto meta = instance<MultiMethod>::make();
	meta->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgs<Symbol, std::string>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Symbol> a(expect<Symbol>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));


		auto t = frame->getNamespace()->lookup(a->value);
		return t->getMeta(*b);
	}));
	meta->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeCollectArgs(),
		[](auto frame, std::vector<instance<>> args)
	{
		instance<Symbol> a(expect<Symbol>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));

		instance<std::string> c = args[2].asType<std::string>();

		auto t = frame->getNamespace()->lookup(a->value);

		auto res = t->getMeta(*b);
		t->addMeta(*b, c);

		return res;
	}));

	ret->define_eval("meta", meta);
}
