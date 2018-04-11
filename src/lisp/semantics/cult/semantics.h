#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** CultSemantics
	******************************************************************************/

	/* Represents the semantic tree for a module.

	This can be thought of as the root AST node for a module. It is the root scope (e.g. it manages
	  dependencies and other scopes it may read from, it holds the symbol table).
	*/
	class CultSemantics
		: public virtual craft::types::Object
		, public craft::types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CultSemantics)
	public:
		struct ReadState
		{
			instance<CultSemantics> _semantics;
			instance<SScope> _scope;
			instance<Sexpr> _sexpr;

			inline instance<> read(size_t index);
		};

		// TODO: Make this a multiMethod
		typedef instance<> (*f_specialFormReader)(ReadState*);

	private:
		instance<lisp::Module> _module;

		// Original Source
		// * Probably: CultLispSyntax
		instance<> _source;

		// List of AST forms in order evaluated
		std::vector<instance<>> _ast;

		//
		// Symbol Tables
		//
	private:
		// TODO: Need to refactor this into a better symbol-binding and module require system

		// TODO: Symbol equality
		std::map<size_t, instance<Binding>> _symbolTable; // Internal table

		// List of Required Modules
		std::vector<instance<lisp::Module>> _modules; // Simple module list

	private:

		CRAFT_LISP_EXPORTED instance<> read_cultLisp(instance<SScope> scope, instance<> syntax);

	public:
		CRAFT_LISP_EXPORTED CultSemantics(instance<lisp::Module> forModule);

		inline instance<lisp::Module> getModule() const { return _module; }
		inline instance<> getSource() const { return _source; }

		CRAFT_LISP_EXPORTED void read(instance<CultLispSyntax> syntax, PSemantics::ReadOptions const* opts);

		// Builtin helpers
	public:
		CRAFT_LISP_EXPORTED void builtin_addSpecialForm(std::string const& symbol_name);
		CRAFT_LISP_EXPORTED void builtin_specialFormReader(std::string const& symbol_name, f_specialFormReader reader);

		CRAFT_LISP_EXPORTED void builtin_addMultiMethod(std::string const& symbol_name);
		CRAFT_LISP_EXPORTED void builtin_attachMultiMethod(std::string const& symbol_name, std::tuple<types::ExpressionStore, types::Function> impl);

		template<typename T>
		inline void builtin_implementMultiMethod(std::string const& symbol_name, T fn)
		{
			builtin_attachMultiMethod(symbol_name, to_expression_and_function(+fn));
		}

		CRAFT_LISP_EXPORTED void builtin_eval(std::string const& contents);

		// SScope
	public:
		CRAFT_LISP_EXPORTED virtual instance<CultSemantics> getSemantics() const override;
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const override;

		// E.g. may enclose over other higher scopes
		CRAFT_LISP_EXPORTED virtual bool isLexicalScope() const override;

		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<> ast) override;
	};


	/******************************************************************************
	** PSemantics for CultSemantics
	******************************************************************************/

	class CultSemanticsProvider
		: public craft::types::Implements<PSemantics>::For<CultSemantics>
	{
		virtual instance<lisp::Module> getModule(instance<> semantics) const override;

		virtual std::vector<craft::types::TypeId> readsFrom() const override;
		virtual instance<> read(instance<> syntax, instance<lisp::Module> into, ReadOptions const* opts) const override;

		virtual std::vector<craft::types::TypeId> transformsFrom() const override;
		virtual instance<> transform(instance<> semantics, instance<lisp::Module> into, instance<> transformationOptions = instance<>()) const override;

		virtual instance<> lookup(instance<> semantics, std::string const&) const override;
	};

	/******************************************************************************
	** SpecialForm
	******************************************************************************/

    /*
		This object represents the semantic object that provides a hook for special/builtin forms.

		It is meant to disguise itself as a normal symbol type as much as possible, in the future
		  it will be a series of multimethods.
	*/
	class SpecialForm
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::SpecialForm);

	protected:
		CRAFT_LISP_EXPORTED SpecialForm();

		friend class CultSemantics;

		CultSemantics::f_specialFormReader _read;

		CRAFT_LISP_EXPORTED instance<> read(instance<CultSemantics>, instance<SScope>, instance<Sexpr>);
	public:
	};
    
}}
