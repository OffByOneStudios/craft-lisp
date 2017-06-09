#pragma once
#include "lisp/common.h"

namespace craft {
namespace features {
namespace languages {
namespace lisp
{

  class SexpSymbol
    : public virtual craft::Object
  {
		CRAFT_OBJECT_DECLARE(SexpSymbol)
	public:
		std::string s;
  };

	struct ParseContext
	{
		std::stack<instance<>> _stack;
		std::shared_ptr<spdlog::logger> _logger;

	};


	CRAFT_LISP_EXPORTED void Evaluate(std::string input, instance<> root, std::shared_ptr<spdlog::logger> logger = nullptr);
}}}}
