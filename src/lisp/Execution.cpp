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

instance<Execution> Execution::execute(instance<SFrame> frame)
{
	auto exc = getCurrent();
	if (!exc)
	{
		if (frame.typeId().isType<Frame>())
		{
			auto scope = frame.asType<Frame>()->getScope();
			exc = instance<Execution>::make(scope->namespace_());
			exc->makeCurrent();
		}
	}

	exc->_stack.push_back(frame);
	frame->beginExecution(exc);

	return exc;
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
	_stack.push_back(_push);
}
void Execution::pop()
{
	if (_stack.size() == 0)
		throw stdext::exception("Execution: stack underflow");

	_stack.pop_back();
}
