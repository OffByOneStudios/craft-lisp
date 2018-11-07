#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Execution.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;

thread_local instance<Execution> Execution::_tl_current;

CRAFT_DEFINE(Execution)
{
	_.defaults();
}


Execution::Execution(instance<Environment> env)
{
	_env = env;
}

void Execution::makeCurrent()
{
	_tl_current = craft_instance();
}
instance<Execution> Execution::getCurrent()
{
	return _tl_current;
}
void Execution::clearFromCurrent()
{
	assert(_tl_current == craft_instance());

	_tl_current = instance<>();
}

instance<Environment> Execution::getEnvironment() const
{
	return _env;
}

std::list<instance<SFrameSection>> const& Execution::stack() const
{
	return _stack;
}
void Execution::push_frame(instance<SFrameSection> _push)
{
	_push->setExecution(craft_instance());
	_stack.push_front(_push);
}
void Execution::pop()
{
	if (_stack.size() == 0)
		throw stdext::exception("Execution: stack underflow");

	_stack.pop_front();
}

instance<> Execution::exec_fromCurrentModule(std::string const& a, types::GenericInvoke const& b)
{
	auto cur = getCurrent();
	auto frame = cur->top();
	if (!frame)
		throw stdext::exception("No Frame");

	auto entries = frame->entries();
	auto module = frame->getEntryModule(entries - 1);
	return module->exec(a, b);
}

instance<> Execution::exec(instance<PSubroutine> a, types::GenericInvoke const & b)
{
	return a->execute(a, b);
}

instance<> Execution::eval(std::string const& a)
{
	throw stdext::exception("Not Implemented");
	return instance<>();
}
