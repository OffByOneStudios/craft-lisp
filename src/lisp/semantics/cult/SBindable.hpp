#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** SBindable
	******************************************************************************/

	/* Provides an interface for objects which can be bound, allowing them to recieve information about how they are being used.
	*/

	class SBindable abstract
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(SBindable, "lisp.bindable", craft::types::FactoryAspectManager);

	public:
		virtual std::string getLocalName() const = 0;

		/* Computed name from localName and broader context.
		*/
		virtual std::string getName() const = 0;

		virtual instance<SBinding> getBinding() const = 0;
		virtual void setBinding(instance<SBinding>) const = 0;
	};
}}
