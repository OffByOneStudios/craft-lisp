#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

#include "lisp/semantics/cult/cult.h"
#include "lisp/backend/BootstrapInterpreter.h"
#include "lisp/library/system/prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

#define lMM semantics->builtin_implementMultiMethod

void core::make_doc_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();


	/*semantics->builtin_addSpecialForm("doc");
	semantics->builtin_specialFormReader("doc",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		
	});*/

	//lMM("doc", [](instance<Symbol> s, instance<std::string> kval) {
	//	auto val = s->trimKeyword();
	//	auto frame = Execution::getCurrent()->topIfOfType<InterpreterFrame>();
	//	auto module_ = frame->getEntryModule(frame->entries() - 1);
	//	auto sem = module_->get<CultSemantics>();
	//	auto bind = sem->lookup(val);

	//	return bind->addMeta("doc", kval);
	//});

	///*lMM("doc/emit", [](instance<Symbol> s) {
	//	auto val = s->trimKeyword();
	//	auto frame = Execution::getCurrent()->topIfOfType<InterpreterFrame>();
	//	auto module_ = frame->getEntryModule(frame->entries() - 1);
	//	auto sem = module_->get<CultSemantics>();
	//	auto bind = sem->lookup(val);

	//	if (!bind) throw stdext::exception("No Such Symbol");

	//	return "";
	//});*/
}
