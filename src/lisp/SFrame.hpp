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
		CRAFT_LISP_EXPORTED virtual void setExecution(instance<Execution>) = 0;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const = 0;
		CRAFT_LISP_EXPORTED virtual instance<> getBackend() const = 0;

		CRAFT_LISP_EXPORTED virtual size_t entries() const = 0;
		CRAFT_LISP_EXPORTED virtual std::string getEntryName(size_t index) const = 0;
		CRAFT_LISP_EXPORTED virtual instance<> getEntryRepresentative(size_t index) const = 0;
		CRAFT_LISP_EXPORTED virtual instance<Module> getEntryModule(size_t index) const = 0;

	public:
		inline bool isExecuting() const
		{
			return (bool)getExecution();
		}
	};
}}
