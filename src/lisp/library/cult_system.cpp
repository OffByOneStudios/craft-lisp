#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

#include "lisp/semantics/cult/cult.h"
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

	auto ret = instance<Module>::make(ns, "builtin:cult.system");
	auto sem = ret->require<CultSemantics>();

	//
	// Multimethods
	//
	sem->builtin_addMultiMethod("truth");

	//
	// Multimethod Implementations
	//

	//
	// Special forms - Reader
	//
	// TODO, make this a multimethod

	sem->builtin_addSpecialForm("define");
	sem->builtin_specialFormReader("define",
		[](CultSemantics::ReadState* rs) -> instance<SCultSemanticNode>
		{
			if (rs->sexpr->cells.size() != 3)
				throw stdext::exception("malformed: (define <symbol> <expr>)");

			instance<Symbol> symbol;
			try
			{
				symbol = library::helper::symbol(rs->sexpr->cells[1]);
			}
			catch (std::exception const& ex)
			{
				throw stdext::exception(ex, "Not Implemented: Dynamic Bindings");
			}

			auto ret = instance<BindSite>::make(symbol, rs->read(2));
			if (!ret->isDynamicBind())
				rs->scope->define(ret->symbolAst().asType<Constant>()->getValue().asType<Symbol>(), ret);
			else
				throw stdext::exception("Not Implemented: Dynamic Bindings");

			return ret;
		});

	sem->builtin_addSpecialForm("do");
	sem->builtin_specialFormReader("do",
		[](CultSemantics::ReadState* rs) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Block>::make();
		auto oldScope = rs->pushScope(ret);

		auto size = rs->sexpr->cells.size();
		ret->preSize(size - 1);
		for (auto i = 1; i < size; i++)
		{
			ret->push(rs->read(i));
		}

		return ret;
	});

	sem->builtin_addSpecialForm("cond");
	sem->builtin_specialFormReader("cond",
		[](CultSemantics::ReadState* rs) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Condition>::make();

		auto size = rs->sexpr->cells.size();
		ret->preSize(size / 2);
		for (auto i = 1; i < size; i += 2)
		{
			ret->push(rs->read(i), rs->read(i + 1));
		}
		if (size % 2 == 0)
			ret->push(instance<>(), rs->read(size - 1));

		return ret;
	});

	sem->builtin_addSpecialForm("while");
	sem->builtin_specialFormReader("while",
		[](CultSemantics::ReadState* rs) -> instance<SCultSemanticNode>
	{
		if (rs->sexpr->cells.size() != 3)
			throw stdext::exception("malformed: (while <cond> <body>)");

		auto ret = instance<Loop>::make(rs->read(1), rs->read(2));

		return ret;
	});

	//
	// Semantics - Interpreter
	//
	sem->builtin_addMultiMethod("eval");
	sem->builtin_implementMultiMethod("eval",
		[](instance<BootstrapInterpreter> interp, instance<Constant> ast) -> instance<>
	{
		return ast->getValue();
	});


	//
	// Semantics - Llvm
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
