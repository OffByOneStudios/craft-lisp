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

InterpreterFrame::SubFrame::SubFrame(instance<SScope> scope, instance<RuntimeSlots> value)
	: scope(scope), value(value), chain(nullptr)
{ }
InterpreterFrame::SubFrame::SubFrame(instance<SScope> scope, instance<RuntimeSlots> value, SubFrame* chain)
	: scope(scope), value(value), chain(chain)
{ }

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

instance<InterpreterFrame> InterpreterFrame::ensureCurrent(instance<BootstrapInterpreter> const& bi)
{
	auto exec = Execution::getCurrent();
	auto top = exec->topIfOfType<InterpreterFrame>();
	if (top) return top;

	auto frame = instance<InterpreterFrame>::make(bi);
	exec->push_frame(frame);
	return frame;
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
instance<> InterpreterFrame::getEntryValue(size_t index) const
{
	return _entries.get_iterator_from_index(index)->value;
}
instance<> InterpreterFrame::getEntryRepresentative(size_t index) const
{
	return _entries.get_iterator_from_index(index)->scope;
}
instance<Module> InterpreterFrame::getEntryModule(size_t index) const
{
	return _entries.get_iterator_from_index(index)->scope->getSemantics()->getModule();
}

size_t InterpreterFrame::getScopeEntryIndex(instance<SScope> scope) const
{
	auto cur = top();

	while (cur != nullptr)
	{
		if (cur->scope == scope)
			return _entries.get_index_from_iterator(_entries.get_iterator_from_pointer(const_cast<SubFrame*>(cur)));

		cur = cur->chain;
	}

	throw stdext::exception("Could not find scope {0} on stack.", scope);
}

InterpreterFrame::SubFrame* InterpreterFrame::top()
{
	return &*_entries.rbegin();
}
InterpreterFrame::SubFrame const* InterpreterFrame::top() const
{
	return &*_entries.rbegin();
}

void InterpreterFrame::push(instance<SScope> scope, instance<RuntimeSlots> value, SubFrame* chain)
{
	_entries.insert(SubFrame(scope, value, chain));
}
void InterpreterFrame::pop()
{
	_entries.erase(--_entries.end());
}

instance<> InterpreterFrame::interp_exec(instance<SCultSemanticNode> node)
{
	return _backend->_fn_system_exec->call_internal({ craft_instance() , node });
}

instance<> InterpreterFrame::interp_call(instance<> fn, types::GenericInvoke const& call, SubFrame* chain)
{
	if (fn.isType<Function>())
	{
		auto fnast = fn.asType<Function>();
		auto rtv = instance<RuntimeSlots>::make(fnast, fnast->argCount());
		auto _this = craft_instance();
		InterpreterFrame::PushSubFrame _hold(_this, fnast, rtv, chain);

		if (call.args.size() != fnast->argCount())
			throw stdext::exception("Interpreter asked to execute function with mismatched arguments ({0} calling {1}).", call.args.size(), fnast->argCount());

		auto count = fnast->argCount();
		for (auto i = 0; i < count; ++i)
		{
			// TODO bindsites manip here instead maybe?
			// TODO argument AST node?
			*rtv->getSlot((instance<>*)(&rtv), i) = call.args[i];
		}

		return interp_exec(fnast->bodyAst());
	}
	if (fn.hasFeature<PSubroutine>())
	{
		auto psub = fn.getFeature<PSubroutine>();
		//TODO Mason Push CFFI Frame
		return psub->execute(fn, call);
	}
	else
	{
		throw stdext::exception("Interpreter asked to call object `{0}` with no subroutine hints.", fn.toString());
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

instance<> BootstrapInterpreter::_special_init(instance<lisp::Module> module, types::GenericInvoke const& call) const
{
	auto sem = module->get<CultSemantics>();
	if (!sem)
		throw stdext::exception("Cannot init {0}: no semantics.", module);

	if (call.args.size() != 0)
		throw stdext::exception("Cannot init {0}: expected 0 args.", module);

	SPDLOG_TRACE(module->getNamespace()->getEnvironment()->log(),
		"BootstrapInterpreter::_special_init\t({0})", module);

	instance<RuntimeSlots> slots = module->moduleValue();

	auto frame = InterpreterFrame::ensureCurrent(craft_instance());
	InterpreterFrame::PushSubFrame _hold(frame, sem, module->moduleValue());

	auto statement_count = sem->countStatements();
	if (!slots)
	{
		slots = instance<RuntimeSlots>::make(module, statement_count);
		module->_value = slots;
	}
	else if (RuntimeSlots::getSize((instance<>*)&slots) != statement_count)
	{
		RuntimeSlots::extend((instance<>*)&slots, statement_count);
	}

	for (auto stmt_i = 0; stmt_i < statement_count; ++stmt_i)
	{
		*RuntimeSlots::getSlot((instance<>*)&slots, stmt_i) = frame->interp_exec(sem->getStatement(stmt_i));
	}

	return slots;
}
instance<> BootstrapInterpreter::_special_append(instance<lisp::Module> module, types::GenericInvoke const& call) const
{
	auto sem = module->get<CultSemantics>();
	if (!sem)
		throw stdext::exception("Cannot append {0}: no semantics.", module);

	if (call.args.size() != 1 && !call.args[0].isType<lisp::Module>())
		throw stdext::exception("Cannot init {0}: expected 1 arg as a lisp::Module.", module);

	SPDLOG_TRACE(module->getNamespace()->getEnvironment()->log(),
		"BootstrapInterpreter::_special_append\t({0}, {1})", module, call.args[0]);

	instance<RuntimeSlots> slots = module->moduleValue();

	auto append_module = call.args[0].asType<lisp::Module>();
	auto append_sem = append_module->require<CultSemantics>();

	if (append_sem->countStatements() == 0)
		return instance<>(); // no op

	auto start = sem->append(append_sem);

	auto frame = InterpreterFrame::ensureCurrent(craft_instance());
	InterpreterFrame::PushSubFrame _hold(frame, sem, module->moduleValue());

	auto statement_count = sem->countStatements();
	RuntimeSlots::extend((instance<>*)&slots, statement_count);
	for (auto stmt_i = start; stmt_i < statement_count; ++stmt_i)
	{
		*RuntimeSlots::getSlot((instance<>*)&slots, stmt_i) = frame->interp_exec(sem->getStatement(stmt_i));
	}

	return RuntimeSlots::getLastSlot((instance<>*)&slots);
}
instance<> BootstrapInterpreter::_special_merge(instance<lisp::Module> module, types::GenericInvoke const& call) const
{
	auto sem = module->get<CultSemantics>();
	if (!sem)
		throw stdext::exception("Cannot merge {0}: no semantics.", module);

	if (call.args.size() != 1 && !call.args[0].isType<lisp::Module>())
		throw stdext::exception("Cannot init {0}: expected 1 arg as a lisp::Module.", module);

	SPDLOG_TRACE(module->getNamespace()->getEnvironment()->log(),
		"BootstrapInterpreter::_special_merge\t({0}, {1})", module, call.args[0]);

	instance<RuntimeSlots> slots = module->moduleValue();

	auto merge_module = call.args[0].asType<lisp::Module>();
	auto merge_sem = merge_module->require<CultSemantics>();

	auto merge_list = sem->append(merge_sem);

	auto frame = InterpreterFrame::ensureCurrent(craft_instance());
	InterpreterFrame::PushSubFrame _hold(frame, sem, module->moduleValue());

	throw stdext::exception("Merge not implemented!.");

	return RuntimeSlots::getLastSlot((instance<>*)&slots);
}

instance<> BootstrapInterpreter::exec(instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const& call)
{
	auto semantics = module->require<CultSemantics>();
	auto binding = semantics->lookup(Symbol::makeSymbol(entry));

	SPDLOG_TRACE(module->getNamespace()->getEnvironment()->log(),
		"BootstrapInterpreter::exec\t({0}, {1}, argc: {2})", module, entry, call.args.size());

	if (!binding)
	{
		if (entry == "::init") return _special_init(module, call);
		if (entry == "::append") return _special_append(module, call);
		if (entry == "::merge") return _special_merge(module, call);


		throw bad_call_error(module, entry);
	}

	auto potFunc = binding->getSite()->valueAst();

	return InterpreterFrame::ensureCurrent(craft_instance())->interp_call(potFunc, call);
}

void BootstrapInterpreter::builtin_validateSpecialForms(instance<lisp::Module> module)
{
	_fn_system_exec = module->get<CultSemantics>()->lookup(Symbol::makeSymbol("exec"))->getSite()->valueAst();

	builtin_truth = module->get<CultSemantics>()->lookup(Symbol::makeSymbol("truth"))->getSite()->valueAst();
	builtin_get = module->get<CultSemantics>()->lookup(Symbol::makeSymbol("get"))->getSite()->valueAst();
	//builtin_set = module->get<CultSemantics>()->lookup(Symbol::makeSymbol("set"))->getSite()->valueAst();
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
