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
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::InterpreterFrame);
	private:
		friend class BootstrapInterpreter;
		friend class InterpreterFrameSection;

		instance<SScope> _scope; // scope node
		instance<RuntimeSlots> _value; // values bound to the scope

		instance<InterpreterFrame> _chain; // Lexical Chain
	public:

		CRAFT_LISP_EXPORTED InterpreterFrame(instance<SScope> _scope, instance<InterpreterFrame> chain = instance<>());
		CRAFT_LISP_EXPORTED InterpreterFrame(instance<SScope> _scope, instance<RuntimeSlots> slots, instance<InterpreterFrame> chain = instance<>());

		CRAFT_LISP_EXPORTED instance<SScope> getScope() const;
		CRAFT_LISP_EXPORTED instance<InterpreterFrame> getLexicalParent() const;

		CRAFT_LISP_EXPORTED instance<RuntimeSlots> getValue();
	};

	class InterpreterFrameSection
		: public virtual craft::types::Object
		, public craft::types::Implements<SFrameSection>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::InterpreterFrameSection);
	private:
		friend class BootstrapInterpreter;

		instance<Execution> _execution;
		instance<BootstrapInterpreter> _backend;

		std::vector<instance<InterpreterFrame>> _frames;

	public:
		CRAFT_LISP_EXPORTED InterpreterFrameSection(instance<BootstrapInterpreter> backend);

		// Pushes a new interpreter frame if it isn't currently there.
		CRAFT_LISP_EXPORTED static instance<InterpreterFrameSection> ensureCurrent(instance<BootstrapInterpreter> const& bi);

		CRAFT_LISP_EXPORTED virtual void setExecution(instance<Execution>) override;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const override;
		CRAFT_LISP_EXPORTED virtual instance<> getBackend() const override;

		CRAFT_LISP_EXPORTED virtual size_t entries() const override;
		CRAFT_LISP_EXPORTED virtual std::string getEntryName(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<> getEntryRepresentative(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<Module> getEntryModule(size_t index) const override;
		CRAFT_LISP_EXPORTED virtual instance<RuntimeSlots> getEntryValue(size_t index) const;

		CRAFT_LISP_EXPORTED instance<InterpreterFrame> getEntry(size_t index) const;

		CRAFT_LISP_EXPORTED instance<InterpreterFrame> findScopeInLexicalChain(instance<SScope>) const;

		CRAFT_LISP_EXPORTED instance<InterpreterFrame> top() const;
		CRAFT_LISP_EXPORTED instance<InterpreterFrame> push(instance<SScope> _scope, instance<InterpreterFrame> chain = instance<>());
		CRAFT_LISP_EXPORTED instance<InterpreterFrame> push(instance<SScope> _scope, instance<RuntimeSlots> slots, instance<InterpreterFrame> chain = instance<>());

		CRAFT_LISP_EXPORTED void pop();

		inline instance<RuntimeSlotReference> slot(instance<Binding> binding)
		{
			auto scope = binding->getScope();
			auto module = getEntryModule(entries() - 1);

			instance<RuntimeSlots> slots;
			if (scope.isType<CultSemantics>())
				slots = scope.asType<CultSemantics>()->getModule()->moduleValue();
			else
				slots = findScopeInLexicalChain(scope)->getValue();

			return instance<RuntimeSlotReference>::make(slots, binding->getIndex());
		}

		// Interpreter helpers
		/*
			This class is used to store the state of the executing interpreter, here we provide
			  helpers for the interpreter code to use
		*/
	public:
		struct Push final
		{
		private:
			instance<InterpreterFrameSection> _interp;
			InterpreterFrame* _frame; // The frame section is already holding this

			Push(Push const&) = delete;
			Push(Push &&) = delete;

		public:
			template<typename ... TArgs>
			inline Push(instance<InterpreterFrameSection>& interp, TArgs &&... args)
				: _interp(interp), _frame(interp->push(std::forward<TArgs>(args)...).get())
			{ }

			inline ~Push() { _interp->pop(); }

			inline InterpreterFrame* frame() { return _frame; }
		};

		// Calls the interpreter's exec function
		CRAFT_LISP_EXPORTED instance<> interp_exec(instance<SCultSemanticNode>);

		// Performs a full call
		CRAFT_LISP_EXPORTED instance<> interp_call(instance<>, types::GenericInvoke const& call, instance<InterpreterFrame> chain = instance<>());
	};

	class BootstrapInterpreter
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BootstrapInterpreter);
	public:
		typedef void(*f_specialFormHandler)();

	private:
		friend class InterpreterFrame;
		friend class InterpreterFrameSection;

		instance<Namespace> _lisp;
		instance<MultiMethod> _fn_system_exec;

	public:
		instance<MultiMethod> builtin_truth;
		instance<MultiMethod> builtin_get;
		instance<MultiMethod> builtin_set;

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
