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
	public:
		struct SubFrame
		{
			instance<SScope> scope; // Scope entry
			std::map<instance<Binding>, instance<>> values; // values bound to the scope

			SubFrame* chain; // Lexical Chain

			CRAFT_LISP_EXPORTED SubFrame(instance<SScope> scope);
			CRAFT_LISP_EXPORTED SubFrame(instance<SScope> scope, SubFrame* chain);
		};

	private:
		friend class BootstrapInterpreter;

		instance<Execution> _execution;
		instance<BootstrapInterpreter> _backend;

		plf::colony<SubFrame> _entries;

	public:
		CRAFT_LISP_EXPORTED InterpreterFrame(instance<> backend);

		CRAFT_LISP_EXPORTED virtual void setExecution(instance<Execution>) override;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const override;
		CRAFT_LISP_EXPORTED virtual instance<> getBackend() const override;

		CRAFT_LISP_EXPORTED virtual size_t entries() const override;
		CRAFT_LISP_EXPORTED virtual std::string getEntryName(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<> getEntryRepresentative(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<Module> getEntryModule(size_t index) const override;

		CRAFT_LISP_EXPORTED SubFrame* top();
		CRAFT_LISP_EXPORTED void push(instance<SScope> scope);
		CRAFT_LISP_EXPORTED void pop();

		// Interpreter helpers
		/*
			This class is used to store the state of the executing interpreter, here we provide
			  helpers for the interpreter code to use
		*/
	public:
		struct PushSubFrame final
		{
		private:
			InterpreterFrame* interp;

			PushSubFrame(PushSubFrame const&) = delete;
			PushSubFrame(PushSubFrame &&) = delete;

		public:
			template<typename ... TArgs>
			inline PushSubFrame(instance<InterpreterFrame>& interp_, TArgs &&... args)
				: interp(interp_.get())
			{ interp->push(std::forward<TArgs>(args)...); }

			inline ~PushSubFrame() { interp->pop(); }
		};

		// Calls the interpreter's exec function
		CRAFT_LISP_EXPORTED instance<> interp_exec(instance<SCultSemanticNode>);

		// Performs a full call
		CRAFT_LISP_EXPORTED instance<> interp_call(instance<>, types::GenericInvoke const& call);
	};

	class BootstrapInterpreter
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BootstrapInterpreter);
	public:
		typedef void(*f_specialFormHandler)();

	private:
		friend class InterpreterFrame;

		instance<Namespace> _lisp;
		instance<MultiMethod> _fn_system_exec;

	public:

		CRAFT_LISP_EXPORTED BootstrapInterpreter(instance<Namespace> lisp);

		CRAFT_LISP_EXPORTED instance<> exec(instance<lisp::Module> module, std::string const& entry, types::GenericInvoke const&);

	public:

		// Ensures the module has everything the interpreter needs
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
