#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/backend.h"

namespace craft {
namespace lisp
{
	class BootstrapInterpreter
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BootstrapInterpreter);
	private:

		instance<Namespace> _lisp;

	public:

		CRAFT_LISP_EXPORTED BootstrapInterpreter(instance<Namespace> lisp);

	};

	class BootstrapInterpreterProvider sealed
		: public types::Implements<PBackend>::For<BootstrapInterpreter>
	{
	public:
		CRAFT_LISP_EXPORTED BootstrapInterpreterProvider();

	public:
		virtual instance<> init(instance<Namespace> env) const override;

		virtual instance<> addModule(instance<> backend_ns, instance<Module>) const override;
		virtual instance<> addFunction(instance<> backend_module, instance<>) const override;

		virtual instance<> exec(instance<lisp::SFrame> frame, instance<> code) const override;
	};
}}
