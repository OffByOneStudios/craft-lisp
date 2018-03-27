#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class SFrame
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::SFrame, "lisp.frame", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<> backend() const = 0;
		CRAFT_LISP_EXPORTED virtual instance<Module> entryModule() const = 0;
	};
}}
