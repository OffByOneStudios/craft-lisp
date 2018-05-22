#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/Environment.h"

using namespace craft;
using namespace craft::lisp;


CRAFT_DEFINE(Environment)
{
	_.defaults();
}

Environment::Environment(std::shared_ptr<spdlog::logger> logger, std::vector<instance<std::string>> argv)
{
	_logger = logger;
	_argv = argv;
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

std::vector<craft::instance<std::string>> Environment::argv()
{
	return _argv;
}
