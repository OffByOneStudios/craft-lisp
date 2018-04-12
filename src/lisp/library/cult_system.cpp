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
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
		{
			if (sexpr->cells.size() != 3)
				throw stdext::exception("malformed: (define <symbol> <expr>)");

			instance<Symbol> symbol;
			try
			{
				symbol = library::helper::symbol(sexpr->cells[1]);
			}
			catch (std::exception const& ex)
			{
				throw stdext::exception(ex, "Not Implemented: Dynamic Bindings");
			}

			auto ret = instance<BindSite>::make(symbol, rs->read(sexpr, 2));
			if (!ret->isDynamicBind())
				rs->scope->define(ret->symbolAst().asType<Constant>()->getValue().asType<Symbol>(), ret);
			else
				throw stdext::exception("Not Implemented: Dynamic Bindings");

			return ret;
		});

	sem->builtin_addSpecialForm("do");
	sem->builtin_specialFormReader("do",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Block>::make(rs->scope);
		CultSemantics::ReadState::PushScope _hold(rs, ret);

		auto size = sexpr->cells.size();
		ret->preSize(size - 1);
		for (auto i = 1; i < size; i++)
		{
			ret->push(rs->read(sexpr, i));
		}

		return ret;
	});

	sem->builtin_addSpecialForm("cond");
	sem->builtin_specialFormReader("cond",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Condition>::make();

		auto size = sexpr->cells.size();
		ret->preSize(size / 2);
		for (auto i = 1; i < size; i += 2)
		{
			ret->push(rs->read(sexpr, i), rs->read(sexpr, i + 1));
		}
		if (size % 2 == 0)
			ret->push(instance<>(), rs->read(sexpr, size - 1));

		return ret;
	});

	sem->builtin_addSpecialForm("while");
	sem->builtin_specialFormReader("while",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		if (sexpr->cells.size() != 3)
			throw stdext::exception("malformed: (while <cond> <body>)");

		auto ret = instance<Loop>::make(rs->read(sexpr, 1), rs->read(sexpr, 2));

		return ret;
	});

	sem->builtin_addSpecialForm("function");
	sem->builtin_specialFormReader("function",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Function>::make();

		return ret;
	});

	sem->builtin_addSpecialForm("variable");
	sem->builtin_specialFormReader("variable",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Variable>::make();

		return ret;
	});

	//
	// Semantics - Interpreter
	//
	sem->builtin_addMultiMethod("exec");
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<Constant> ast) -> instance<>
	{
		return ast->getValue();
	});
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<Block> ast) -> instance<>
	{
		InterpreterFrame::PushSubFrame _hold(interp, ast);

		auto count = ast->statementCount();
		instance<> last_res;
		for (auto i = 0; i < count; i++)
		{
			last_res = interp->interp_exec(ast->statementAst(i));
		}

		return last_res;
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
