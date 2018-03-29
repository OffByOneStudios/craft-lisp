#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/backend/backend.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{
	class InterpreterFrame
		: public virtual craft::types::Object
		, public Implements<SFrame>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::InterpreterFrame);
	private:
		friend class BootstrapInterpreter;

		instance<Execution> _execution;
		instance<> _backend;
		instance<> _entryModule;

		instance<SScope> _scope;
		std::map<instance<SBinding>, instance<>> _values;

	public:
		CRAFT_LISP_EXPORTED InterpreterFrame(instance<Execution> exec, instance<> backend, instance<Module> semanticsOwner, instance<SSubroutine>);

		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const;
		CRAFT_LISP_EXPORTED virtual instance<> backend() const;
		CRAFT_LISP_EXPORTED virtual instance<Module> entryModule() const;
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

		CRAFT_LISP_EXPORTED instance<> exec_cult(instance<SSubroutine>, GenericCall const&);

		CRAFT_LISP_EXPORTED instance<> exec(instance<lisp::Module> module, std::string const& entry, GenericCall const&);

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

		virtual instance<> exec(instance<> backend, instance<lisp::Module> module, std::string const& entry, GenericCall const&) const override;
	};
}}

inline craft::types::TypeId craft::types::type<craft::lisp::BootstrapInterpreter>::typeId()
{
  return craft::lisp::BootstrapInterpreter::craft_s_typeId();
};
