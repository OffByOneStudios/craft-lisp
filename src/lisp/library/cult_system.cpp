#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

#include "lisp/semantics/cult/cult.h"
#include "lisp/backend/BootstrapInterpreter.h"
//#include "lisp/backend/llvm/llvm_internal.h"

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

	auto ret = instance<Module>::make(ns, "builtin:cult.system");
	auto sem = instance<CultSemantics>::make(ret);
	ret->builtin_setSemantics(sem);

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

			return instance<BindSite>::make(symbol, rs->read(sexpr, 2));
		});

	sem->builtin_addSpecialForm("do");
	sem->builtin_specialFormReader("do",
		[](CultSemantics::ReadState* rs, instance<Sexpr> sexpr) -> instance<SCultSemanticNode>
	{
		auto ret = instance<Block>::make();

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
		[](instance<InterpreterFrame> interp, instance<GetValue> ast) -> instance<>
	{
		return *interp->slot(ast->getBinding());
	});
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<BindSite> ast) -> instance<>
	{
		auto symbol = ast->getStaticSymbol(); // TODO execute dynamic binding
		auto binding = SScope::findScope(ast)->lookup_recurse(symbol);

		auto value = interp->interp_exec(ast->valueAst());

		if (!ast->isAttachSite())
			*interp->slot(binding) = value;
		return value;
	});
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<CallSite> ast) -> instance<>
	{
		auto callee = interp->interp_exec(ast->calleeAst());

		auto count = ast->argCount();
		GenericInvoke invoke(count);
		for (auto i = 0; i < count; i++)
		{
			invoke.args.push_back(interp->interp_exec(ast->argAst(i)));
		}

		auto value = interp->interp_call(callee, invoke);

		return value;
	});
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<Block> ast) -> instance<>
	{
		InterpreterFrame::PushSubFrame _hold(interp, ast, instance<RuntimeSlots>::make(ast, 2));

		auto count = ast->statementCount();
		instance<> last_res;
		for (auto i = 0; i < count; i++)
		{
			last_res = interp->interp_exec(ast->statementAst(i));
		}

		return last_res;
	});
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<Condition> ast) -> instance<>
	{
		auto count = ast->branchCount();
		for (auto i = 0; i < count; i++)
		{
			auto cond = interp->interp_exec(ast->branchConditionAst(i));
			if (cond) // TODO truth
			{
				return interp->interp_exec(ast->branchAst(i));
			}
		}

		return interp->interp_exec(ast->branchDefaultAst());
	});
	sem->builtin_implementMultiMethod("exec",
		[](instance<InterpreterFrame> interp, instance<> ast) -> instance<>
	{
		return ast;
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
