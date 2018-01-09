#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class SFrame
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_ASPECT_DECLARE(craft::lisp::SFrame, "lisp.frame", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const = 0;
		CRAFT_LISP_EXPORTED virtual instance<Execution> execution() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<SFrame> parent() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<SFrame> lexical_parent() const = 0;
	};
}}
