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
		CRAFT_LISP_EXPORTED virtual instance<Namespace> getNamespace() const = 0;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const = 0;

		CRAFT_LISP_EXPORTED virtual instance<SFrame> getPrevious() const = 0;
		CRAFT_LISP_EXPORTED virtual instance<SFrame> getLexicalParent() const = 0;

		CRAFT_LISP_EXPORTED virtual void beginExecution(instance<Execution>) = 0;
	};
}}
