#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

#include "lisp/semantics/cult/cult_semantics.h"
#include "lisp/backend/BootstrapInterpreter.h"
#include "lisp/backend/llvm/llvm_internal.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library::helper;


instance<Module> library::make_module_builtin_cult_system(instance<Namespace> ns)
{
	//
	// Aquire objects, set up returns
	//
	auto backend_interp = ns->get<BootstrapInterpreter>();
	auto backend_llvm = ns->get<LlvmBackend>();

	auto ret = instance<Module>::make(ns);
	auto sem = instance<CultSemantics>::make(ret);

	//
	// Multimethods
	//
	sem->builtin_addMultiMethod("truth");
	sem->builtin_addMultiMethod("read"); // read multi-method rather than using special form read

	//
	// Multimethod Implementations
	//

	//
	// Special forms
	//
	auto sf_define = sem->builtin_addSpecialForm("define");

	//
	// Special forms - Reader
	//
	// TODO, make this a multimethod

	sem->builtin_implementMultiMethod("read",
		"define",
		[](instance<CultSemantics> semantics, instance<SScope> scope, instance<Sexpr> sexpr)
		{
			if (sexpr->cells.size() != 3)
				throw stdext::exception("malformed: (define <symbol> <expr>)");

			auto name = sexpr->cells[1];
			auto object = sexpr->cells[2];

			std::string name_value = symbol(name);

			object = scope->environment()->read(scope, object);

			auto binding = scope->define(name_value, object);

			auto ret = instance<Sexpr>::make();
			ret->cells.push_back(head);
			ret->cells.push_back(binding);
			return ret;
		});

	//
	// Special forms - Interpreter
	//
	// TODO, make this a multimethod


	//
	// Special forms - Llvm
	//
	// TODO, make this a multimethod


	//
	// Validate, set state
	//
	backend_interp->builtin_validateSpecialForms(ret);
	//backend_llvm->builtin_validateSpecialForms(ret);

	ret->builtin_setSemantics(sem);
	return ret;
}
