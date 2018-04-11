#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/backend.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	class InterpreterFrame
		: public virtual craft::types::Object
		, public craft::types::Implements<SFrame>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::InterpreterFrame);
	private:
		friend class BootstrapInterpreter;

		instance<Execution> _execution;
		instance<> _backend;

		struct _Entry
		{
			instance<Function> _function;
			std::vector<instance<>> _state;
			instance<SScope> _scope; // if any
			std::map<instance<Binding>, instance<>> _values; // values bound to the scope
		};

		std::vector<_Entry> _entries;

		_Entry const& _getEntry(size_t) const;

	public:
		CRAFT_LISP_EXPORTED InterpreterFrame(instance<> backend);

		CRAFT_LISP_EXPORTED virtual void setExecution(instance<Execution>) override;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const override;
		CRAFT_LISP_EXPORTED virtual instance<> getBackend() const override;

		CRAFT_LISP_EXPORTED virtual size_t entries() const override;
		CRAFT_LISP_EXPORTED virtual std::string getEntryName(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<> getEntryRepresentative(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<Module> getEntryModule(size_t index) const override;

		CRAFT_LISP_EXPORTED void pushEntry(instance<Function> function, types::GenericInvoke const& args);
		CRAFT_LISP_EXPORTED instance<>& topState();
	};

	class BootstrapInterpreter
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BootstrapInterpreter);
	public:
		typedef void(*f_specialFormHandler)();

	private:
		instance<Namespace> _lisp;

	public:

		CRAFT_LISP_EXPORTED BootstrapInterpreter(instance<Namespace> lisp);

		CRAFT_LISP_EXPORTED instance<> exec_cult(instance<>, types::GenericInvoke const&);

		CRAFT_LISP_EXPORTED instance<> exec(instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const&);

	public:

		CRAFT_LISP_EXPORTED void builtin_addSpecialFormHandler(std::string const&, f_specialFormHandler handler);

		CRAFT_LISP_EXPORTED void builtin_validateSpecialForms(instance<lisp::Module> module);
	};

	class BootstrapInterpreterProvider final
		: public types::Implements<PBackend>::For<BootstrapInterpreter>
		, public types::Implements<PExecutor>::For<BootstrapInterpreter>
	{
	public:
		CRAFT_LISP_EXPORTED BootstrapInterpreterProvider();

	public:
		virtual instance<> init(instance<Namespace> env) const override;

		virtual instance<> exec(instance<> backend, instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const&) const override;

		virtual void resume(instance<> backend) const override;
	};
}}

inline craft::types::TypeId craft::types::type<craft::lisp::BootstrapInterpreter>::typeId()
{
  return craft::lisp::BootstrapInterpreter::craft_s_typeId();
};
