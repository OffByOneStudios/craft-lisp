#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/backend.h"
#include "lisp/semantics/cult/cult_semantics.h"
#include "lisp/backend/BootstrapInterpreter.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** InterpreterFrame
******************************************************************************/

CRAFT_DEFINE(InterpreterFrame)
{
	_.use<SFrame>().byCasting();

	_.defaults();
}

InterpreterFrame::InterpreterFrame(instance<> backend)
{
	_backend = backend;
}

InterpreterFrame::_Entry const& InterpreterFrame::_getEntry(size_t i) const
{
	if (i >= _entries.size())
		throw stdext::exception("Frame entry index out of range.");

	return _entries[i];
}

void InterpreterFrame::setExecution(instance<Execution> exec)
{
	_execution = exec;
}

instance<Execution> InterpreterFrame::getExecution() const
{
	return _execution;
}

instance<> InterpreterFrame::getBackend() const
{
	return _backend;
}

size_t InterpreterFrame::entries() const
{
	return _entries.size();
}
std::string InterpreterFrame::getEntryName(size_t index) const
{

}
instance<> InterpreterFrame::getEntryRepresentative(size_t index) const
{
	return _getEntry(index)._function;
}
instance<Module> InterpreterFrame::getEntryModule(size_t index) const
{
	return _getEntry(index)._function->;
}

void InterpreterFrame::pushEntry(instance<Function> function, types::GenericInvoke const& args)
{

}
instance<>& InterpreterFrame::topState()
{

}

/******************************************************************************
** BootstrapInterpreter
******************************************************************************/

CRAFT_DEFINE(BootstrapInterpreter)
{
	_.use<PBackend>().singleton<BootstrapInterpreterProvider>();

	_.defaults();
}

BootstrapInterpreter::BootstrapInterpreter(instance<Namespace> lisp)
{
	_lisp = lisp;
}

instance<> BootstrapInterpreter::exec_cult(instance<SSubroutine> s, types::GenericInvoke const& args)
{
	if (code.typeId().hasFeature<SBinding>())
		return code.asFeature<SBinding>()->getValue(frame);
	else if (code.typeId().isType<Sexpr>())
	{
		instance<Sexpr> expr = code;

		if (expr->cells.size() == 0)
			throw stdext::exception("Unquoted empty list.");

		// -- Evaluate Head --
		instance<> head = expr->car();

		head = exec(frame, head);

		// -- Evaluate Special Forms --
		if (head.typeId().isType<SpecialForm>())
			return head.asType<SpecialForm>()->eval(frame, expr);

		// -- Evaluate Subroutine --
		if (!head.hasFeature<PSubroutine>())
		{
			throw stdext::exception("Cannot call `{0}`", head.toString());
		}

		auto subroutine_provider = head.getFeature<PSubroutine>();

		std::vector<instance<>> sub_expr_values;
		sub_expr_values.reserve(expr->cells.size() - 1);

		for (auto it = expr->cells.begin() + 1, end = expr->cells.end(); it != end; ++it)
		{
			sub_expr_values.push_back(exec(frame, *it));
		}

		// -- Call --
		frame = subroutine_provider->call_frame(head);
		Execution::execute(frame);
		return subroutine_provider->call(head, frame, sub_expr_values);
	}
	else
		return code;
}

instance<> BootstrapInterpreter::exec(instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const& call)
{
	auto semantics = module->require<CultSemantics>();

	auto frame = instance<InterpreterFrame>::make(craft_instance());

	Execution::getCurrent()->push_frame(frame);
}

/******************************************************************************
** BootstrapInterpreterProvider
******************************************************************************/

BootstrapInterpreterProvider::BootstrapInterpreterProvider()
{

}

instance<> BootstrapInterpreterProvider::init(instance<Namespace> ns) const
{
	return instance<BootstrapInterpreter>::make(ns);
}

instance<> BootstrapInterpreterProvider::exec(instance<> backend, instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const& call) const
{
	return backend.asType<BootstrapInterpreter>()->exec(module, entry, call);
}
