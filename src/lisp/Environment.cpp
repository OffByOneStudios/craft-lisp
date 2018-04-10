#include "common.h"
#include "lisp.h"
#include "Environment.h"

using namespace craft::lisp;


CRAFT_DEFINE(Environment)
{
	_.defaults();
}

Environment::Environment(std::shared_ptr<spdlog::logger> logger)
{
	_logger = logger;
}

void Environment::craft_setupInstance()
{
	Object::craft_setupInstance();

	ns_user = instance<Namespace>::make(craft_instance());
}

std::shared_ptr<spdlog::logger> Environment::log()
{
	return _logger;
}
