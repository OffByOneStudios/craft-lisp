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


Execution::Execution(instance<Namespace> ns)
{
	_namespace = ns;
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

instance<Namespace> Execution::getNamespace() const
{
	return _namespace;
}

std::vector<instance<SFrame>> const& Execution::stack() const
{
	return _stack;
}
void Execution::push_frame(instance<SFrame> _push)
{
	_push->setExecution(craft_instance());
	_stack.push_back(_push);
}
void Execution::pop()
{
	if (_stack.size() == 0)
		throw stdext::exception("Execution: stack underflow");

	_stack.pop_back();
}

instance<> Execution::exec_fromCurrentModule(std::string const& a, types::GenericInvoke const& b)
{
	throw stdext::exception("Not Implemented");
	return instance<>();
}

instance<> Execution::exec(instance<PSubroutine> a, types::GenericInvoke const & b)
{
	throw stdext::exception("Not Implemented");
	return instance<>();
}

instance<> Execution::eval(std::string const& a)
{
	throw stdext::exception("Not Implemented");
	return instance<>();
}
