#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Execution.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;


CRAFT_OBJECT_DEFINE(Execution)
{
	_.defaults();
}


Execution::Execution(instance<Environment> env, instance<Namespace> ns)
{
	_environment = env;
	_namespace = ns;
}

instance<Namespace> Execution::namespace_() const
{
	return _namespace;
}
instance<Environment> Execution::environment() const
{
	return _environment;
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
