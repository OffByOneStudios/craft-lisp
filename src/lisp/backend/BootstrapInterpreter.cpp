#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/backend.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/backend/BootstrapInterpreter.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** InterpreterFrame::SubFrame
******************************************************************************/

InterpreterFrame::SubFrame::SubFrame(instance<SScope> scope)
{
	this->scope = scope;
}
InterpreterFrame::SubFrame::SubFrame(instance<SScope> scope, SubFrame* chain)
{
	this->scope = scope;
	this->chain = chain;
}

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
	return "";
}
instance<> InterpreterFrame::getEntryRepresentative(size_t index) const
{
	return _entries.get_iterator_from_index(index)->scope;
}
instance<Module> InterpreterFrame::getEntryModule(size_t index) const
{
	return _entries.get_iterator_from_index(index)->scope->getSemantics()->getModule();
}

InterpreterFrame::SubFrame* InterpreterFrame::top()
{
	return &*_entries.rbegin();
}

void InterpreterFrame::push(instance<SScope> scope)
{
	_entries.insert(SubFrame(scope));
}
void InterpreterFrame::pop()
{
	_entries.erase(--_entries.end());
}

instance<> InterpreterFrame::interp_exec(instance<SCultSemanticNode> node)
{
	return _backend->_fn_system_exec->call_internal({ craft_instance() , node });
}

instance<> InterpreterFrame::interp_call(instance<> fn, types::GenericInvoke const& call)
{
	if (fn.hasFeature<PSubroutine>())
	{
		auto psub = fn.getFeature<PSubroutine>();
		return psub->execute(fn, call);
	}
	else
	{
		throw stdext::exception("Interpreter asked to call object (`{0}`) with no subroutine hints.", fn.toString());
	}
}

/******************************************************************************
** BootstrapInterpreter
******************************************************************************/

CRAFT_DEFINE(BootstrapInterpreter)
{
	_.use<PBackend>().singleton<BootstrapInterpreterProvider>();
	_.use<PExecutor>().singleton<BootstrapInterpreterProvider>();

	_.defaults();
}

BootstrapInterpreter::BootstrapInterpreter(instance<Namespace> lisp)
{
	_lisp = lisp;
}

instance<> BootstrapInterpreter::exec(instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const& call)
{
	auto semantics = module->require<CultSemantics>();
	auto binding = semantics->lookup(Symbol::makeSymbol(entry));

	if (!binding)
	{
		if (entry == "init")


		throw stdext::exception("Execution entry point `{0}` not found in `{1}`.", entry, module);
	}

	auto potFunc = binding->getSite()->symbolAst();

	auto frame = instance<InterpreterFrame>::make(craft_instance());
	Execution::getCurrent()->push_frame(frame);

	return frame->interp_call(potFunc, call);
}

void BootstrapInterpreter::builtin_validateSpecialForms(instance<lisp::Module> module)
{
	_fn_system_exec = module->get<CultSemantics>()->lookup(Symbol::makeSymbol("exec"))->getSite()->valueAst();
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
