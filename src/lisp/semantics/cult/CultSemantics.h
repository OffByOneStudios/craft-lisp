#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{
	/* Represents the semantic tree for a module.

	This can be thought of as the root AST node for a module. It is the root scope (e.g. it manages
	  dependencies and other scopes it may read from, it holds the symbol table).
	*/
	class CultSemantics
		: public virtual craft::types::Object
		, public Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CultSemantics)
	public:
		// TODO: Make this a multiMethod
		typedef instance<> (*f_specialFormReader)(instance<CultSemantics>, instance<SScope> scope, instance<Sexpr>);

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
		std::map<size_t, instance<>> _symbolTable; // Internal table

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
		CRAFT_LISP_EXPORTED void builtin_addMultiMethod(std::string const& symbol_name);

		CRAFT_LISP_EXPORTED instance<Symbol> builtin_addSpecialForm(std::string const& symbol_name, f_specialFormReader reader);

		CRAFT_LISP_EXPORTED void builtin_eval(std::string const& contents);

		// SScope
	public:
	};


	/******************************************************************************
	** PSemantics for CultSemantics
	******************************************************************************/

	class CultSemanticsProvider
		: public types::Implements<PSemantics>::For<CultSemantics>
	{
		virtual instance<lisp::Module> getModule(instance<> semantics) const override;

		virtual std::vector<craft::types::TypeId> readsFrom() const override;
		virtual instance<> read(instance<> syntax, instance<lisp::Module> into, ReadOptions const* opts) const override;

		virtual std::vector<craft::types::TypeId> transformsFrom() const override;
		virtual instance<> transform(instance<> semantics, instance<lisp::Module> into, instance<> transformationOptions = instance<>()) const override;

		virtual instance<> lookup(instance<> semantics, std::string const&) const override;
	};

}}
