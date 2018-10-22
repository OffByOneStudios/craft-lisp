#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/backend.h"
#include "lisp/semantics/cult/cult.h"
#include "lisp/backend/BootstrapInterpreter.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

/******************************************************************************
** InterpreterFrame
******************************************************************************/

CRAFT_DEFINE(InterpreterFrame)
{
	_.defaults();
}

InterpreterFrame::InterpreterFrame(instance<SScope> scope, instance<InterpreterFrame> chain)
	: _scope(scope)
	, _value(instance<RuntimeSlots>::make(scope, scope->getSlotCount()))
	, _chain(chain)
{
}
InterpreterFrame::InterpreterFrame(instance<SScope> scope, instance<RuntimeSlots> slots, instance<InterpreterFrame> chain)
	: _scope(scope)
	, _value(slots ? slots : instance<RuntimeSlots>::make(scope, scope->getSlotCount()))
	, _chain(chain)
{
	assert(_value->getSize() >= scope->getSlotCount());
}

instance<SScope> InterpreterFrame::getScope() const
{
	return _scope;
}
instance<InterpreterFrame> InterpreterFrame::getLexicalParent() const
{
	return _chain;
}

instance<RuntimeSlots> InterpreterFrame::getValue()
{
	return _value;
}

/******************************************************************************
** InterpreterFrameSection
******************************************************************************/

CRAFT_DEFINE(InterpreterFrameSection)
{
	_.use<SFrameSection>().byCasting();

	_.defaults();
}

InterpreterFrameSection::InterpreterFrameSection(instance<BootstrapInterpreter> backend)
{
	_backend = backend;
}

instance<InterpreterFrameSection> InterpreterFrameSection::ensureCurrent(instance<BootstrapInterpreter> const& bi)
{
	auto exec = Execution::getCurrent();
	auto top = exec->topIfOfType<InterpreterFrameSection>();
	if (top) return top;

	auto frame = instance<InterpreterFrameSection>::make(bi);
	exec->push_frame(frame);
	return frame;
}

void InterpreterFrameSection::setExecution(instance<Execution> exec)
{
	_execution = exec;
}

instance<Execution> InterpreterFrameSection::getExecution() const
{
	return _execution;
}

instance<> InterpreterFrameSection::getBackend() const
{
	return _backend;
}

size_t InterpreterFrameSection::entries() const
{
	return _frames.size();
}
std::string InterpreterFrameSection::getEntryName(size_t index) const
{
	return "";
}
instance<> InterpreterFrameSection::getEntryRepresentative(size_t index) const
{
	return _frames[index]->_scope;
}
instance<Module> InterpreterFrameSection::getEntryModule(size_t index) const
{
	return _frames[index]->_scope->getSemantics()->getModule();
}
instance<RuntimeSlots> InterpreterFrameSection::getEntryValue(size_t index) const
{
	return _frames[index]->_value;
}

instance<InterpreterFrame> InterpreterFrameSection::findScopeInLexicalChain(instance<SScope> scope) const
{
	auto cur = _frames.back();

	while (cur)
	{
		if (cur->_scope == scope)
			return cur;

		cur = cur->_chain;
	}

	throw stdext::exception("Could not find scope {0} on stack.", scope);
}

instance<InterpreterFrame> InterpreterFrameSection::top() const
{
	return _frames.back();
}

instance<InterpreterFrame> InterpreterFrameSection::push(instance<SScope> scope, instance<InterpreterFrame> chain)
{
	auto ret = instance<InterpreterFrame>::make(scope, chain);
	_frames.push_back(ret);
	return ret;
}
instance<InterpreterFrame> InterpreterFrameSection::push(instance<SScope> scope, instance<RuntimeSlots> value, instance<InterpreterFrame> chain)
{
	auto ret = instance<InterpreterFrame>::make(scope, value, chain);
	_frames.push_back(ret);
	return ret;
}
void InterpreterFrameSection::pop()
{
	_frames.pop_back();
}

instance<> InterpreterFrameSection::interp_exec(instance<SCultSemanticNode> node)
{
	return _backend->_fn_system_exec->call_internal({ craft_instance() , node });
}

instance<> InterpreterFrameSection::interp_call(instance<> fn, types::GenericInvoke const& call, instance<InterpreterFrame> chain)
{
	if (fn.isType<Function>())
	{
		auto fnast = fn.asType<Function>();
		InterpreterFrameSection::Push _hold(craft_instance(), fnast, chain);

		if (call.args.size() != fnast->argCount())
			throw stdext::exception("Interpreter asked to execute function with mismatched arguments ({0} calling {1}).", call.args.size(), fnast->argCount());

		auto rtv = _hold.frame()->getValue();
		auto count = fnast->argCount();
		for (auto i = 0; i < count; ++i)
		{
			// TODO bindsites manip here instead maybe?
			// TODO argument AST node?
			auto a = fnast->argAst(i);
			if (!(a && a.isType<BindSite>())) throw stdext::exception("Malformed Argument");
			auto val = a.asType<BindSite>()->valueAst();
			if (!(val && val.isType<Variable>())) throw stdext::exception("Malformed Argument");

			auto var = val.asType<Variable>();

			auto type = var->typeAst();
			if (type)
			{
				auto lHs = ExpressionConcrete(type.asType<Resolve>()->getConstantValue().asType<Graph::Node>().get());
				
				auto rHs = ExpressionConcrete(call.args[i].typeId());
				if (!types::isSubtype(&lHs, &rHs)) throw stdext::exception("Interpreter asked to execute function with mismatched argument: {0}", i);
			}
			
			*(rtv->getSlot(i)) = call.args[i];
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

	auto statement_count = sem->countStatements();

	instance<RuntimeSlots> slots = module->moduleValue();
	if (!slots)
		slots = instance<RuntimeSlots>::make(sem, statement_count);
	else if (slots->getSize() != statement_count)
		slots->extend(statement_count);

	auto frame = InterpreterFrameSection::ensureCurrent(craft_instance());
	InterpreterFrameSection::Push _hold(frame, sem, slots);

	module->_value = slots = _hold.frame()->getValue();

	for (auto stmt_i = 0; stmt_i < statement_count; ++stmt_i)
	{
		*slots->getSlot(stmt_i) = frame->interp_exec(sem->getStatement(stmt_i));
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
	auto statement_count = sem->countStatements();
	slots->extend(statement_count);

	auto frame = InterpreterFrameSection::ensureCurrent(craft_instance());
	InterpreterFrameSection::Push _hold(frame, sem, slots);

	for (auto stmt_i = start; stmt_i < statement_count; ++stmt_i)
	{
		*slots->getSlot(stmt_i) = frame->interp_exec(sem->getStatement(stmt_i));
	}

	return slots->getLastSlot();
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

	auto frame = InterpreterFrameSection::ensureCurrent(craft_instance());
	InterpreterFrameSection::Push _hold(frame, sem, slots);

	throw stdext::exception("Merge not implemented!.");

	return slots->getLastSlot();
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

	return InterpreterFrameSection::ensureCurrent(craft_instance())->interp_call(potFunc, call);
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
