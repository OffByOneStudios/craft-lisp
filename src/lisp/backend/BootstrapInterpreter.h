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
			instance<RuntimeSlots> value; // values bound to the scope

			SubFrame* chain; // Lexical Chain

			CRAFT_LISP_EXPORTED SubFrame(instance<SScope> scope, instance<RuntimeSlots> value);
			CRAFT_LISP_EXPORTED SubFrame(instance<SScope> scope, instance<RuntimeSlots> value, SubFrame* chain);
		};

	private:
		friend class BootstrapInterpreter;

		instance<Execution> _execution;
		instance<BootstrapInterpreter> _backend;

		plf::colony<SubFrame> _entries;

	public:
		CRAFT_LISP_EXPORTED InterpreterFrame(instance<> backend);

		// Pushes a new interpreter frame if it isn't currently there.
		CRAFT_LISP_EXPORTED static instance<InterpreterFrame> ensureCurrent(instance<BootstrapInterpreter> const& bi);

		CRAFT_LISP_EXPORTED virtual void setExecution(instance<Execution>) override;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const override;
		CRAFT_LISP_EXPORTED virtual instance<> getBackend() const override;

		CRAFT_LISP_EXPORTED virtual size_t entries() const override;
		CRAFT_LISP_EXPORTED virtual std::string getEntryName(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<> getEntryRepresentative(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<Module> getEntryModule(size_t index) const override;

		CRAFT_LISP_EXPORTED size_t getScopeEntryIndex(instance<SScope>) const;
		CRAFT_LISP_EXPORTED virtual instance<> getEntryValue(size_t index) const;

		CRAFT_LISP_EXPORTED SubFrame* top();
		CRAFT_LISP_EXPORTED SubFrame const* top() const;
		CRAFT_LISP_EXPORTED void push(instance<SScope> scope, instance<RuntimeSlots> value, SubFrame* = nullptr);
		CRAFT_LISP_EXPORTED void pop();

		inline instance<>* slot(instance<Binding> binding)
		{
			auto scope = binding->getScope();
			auto module = getEntryModule(entries() - 1);

			instance<RuntimeSlots> slots;
			if (scope.isType<CultSemantics>())
				slots = scope.asType<CultSemantics>()->getModule()->moduleValue();
			else
				slots = getEntryValue(getScopeEntryIndex(scope));

			return slots->getSlot((instance<>*)&slots, binding->getIndex());
		}

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

	private:
		CRAFT_LISP_EXPORTED instance<> _special_init(instance<lisp::Module> module, types::GenericInvoke const&) const;
		CRAFT_LISP_EXPORTED instance<> _special_append(instance<lisp::Module> module, types::GenericInvoke const&) const;
		CRAFT_LISP_EXPORTED instance<> _special_merge(instance<lisp::Module> module, types::GenericInvoke const&) const;

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
	};
}}
