#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{

	class Environment
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Environment);
	private:

		std::shared_ptr<spdlog::logger> _logger;
		std::vector<instance<std::string>> _argv;
	public:

		instance<Namespace> ns_user;

	public:

		CRAFT_LISP_EXPORTED Environment(std::shared_ptr<spdlog::logger> logger, std::vector<instance<std::string>> argv = {});
		CRAFT_LISP_EXPORTED void craft_setupInstance();

		// Global Logger
		CRAFT_LISP_EXPORTED std::shared_ptr<spdlog::logger> log();
		// Global Argv
		CRAFT_LISP_EXPORTED std::vector<instance<std::string>> argv();
	};

}}
