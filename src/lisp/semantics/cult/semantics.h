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
		, public craft::types::Implements<SCultSemanticNode>
		, public craft::types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CultSemantics)
	public:
		struct ReadState final
		{
			instance<CultSemantics> semantics;

			inline instance<SCultSemanticNode> read(instance<Sexpr>, size_t index);
			inline std::vector<instance<SCultSemanticNode>> readAll(instance<Sexpr>, size_t start_index = 1);
		};

		// TODO: Make this a multiMethod
		typedef instance<SCultSemanticNode> (*f_specialFormReader)(ReadState*, instance<Sexpr>);

	private:
		instance<lisp::Module> _module;

		// Original Source
		// * Probably: CultLispSyntax
		instance<> _source;

		// List of AST forms in order evaluated
		std::vector<instance<SCultSemanticNode>> _ast;

		//
		// Symbol Tables
		//
	private:
		// TODO: Need to refactor this into a better symbol-binding and module require system

		// TODO: Replace with namespace and namelayer aware solution.
		_SimpleSymbolTableBindings _bindings;

		// List of Required Modules
		std::vector<instance<lisp::Module>> _modules; // Simple module list
		std::vector<instance<lisp::Module>> _modules_cache; // module cache

	private:

		CRAFT_LISP_EXPORTED void rebuildModulesCache();

		CRAFT_LISP_EXPORTED instance<SCultSemanticNode> read_cultLisp(ReadState* rs, instance<> syntax);

	public:
		CRAFT_LISP_EXPORTED CultSemantics(instance<lisp::Module> forModule);

		inline instance<lisp::Module> getModule() const { return _module; }
		inline instance<> getSource() const { return _source; }

		CRAFT_LISP_EXPORTED void readPrepDefaults();
		CRAFT_LISP_EXPORTED void read(instance<CultLispSyntax> syntax, PSemantics::ReadOptions const* opts);

		CRAFT_LISP_EXPORTED std::vector<instance<Binding>> search(std::string const& search) const;

		inline size_t countStatements() const { return _ast.size(); }
		inline instance<SCultSemanticNode> getStatement(size_t index) const { return _ast[index]; }

		inline size_t countBindings() const { return _bindings.bindings.size(); }
		inline instance<Binding> lookup(size_t index) const { return _bindings.bindings[index]; }

		// Requirements:
	public:
		CRAFT_LISP_EXPORTED void importModule(instance<Module> m);

		// Compute merge/append and return execution point
	public:
		CRAFT_LISP_EXPORTED size_t append(instance<CultSemantics> sem);
		CRAFT_LISP_EXPORTED std::vector<size_t> merge(instance<CultSemantics> sem);

		// Builtin helpers
	public:
		CRAFT_LISP_EXPORTED void builtin_addSpecialForm(std::string const& symbol_name);
		CRAFT_LISP_EXPORTED void builtin_specialFormReader(std::string const& symbol_name, f_specialFormReader reader);

		CRAFT_LISP_EXPORTED void builtin_addMultiMethod(std::string const& symbol_name, types::cpp::Multimethod<types::ExpressionDispatcher>* existing = nullptr);
		CRAFT_LISP_EXPORTED void builtin_attachMultiMethod(std::string const& symbol_name, std::tuple<types::ExpressionStore, types::Function> impl);

		CRAFT_LISP_EXPORTED void builtin_defineConstant(std::string const& symbol_name, instance<>);

		template<typename T>
		inline void builtin_implementMultiMethod(std::string const& symbol_name, T fn)
		{
			builtin_attachMultiMethod(symbol_name, to_expression_and_function(+fn));
		}

		template<typename T>
		inline void builtin_defineType(std::string const& symbol_name)
		{
			builtin_defineConstant(symbol_name, instance<types::Graph::Node>::makeFromPointerAndMemoryManager(types::cpptype<T>::typeDesc().asId(), &graph()));
		}

		CRAFT_LISP_EXPORTED void builtin_eval(std::string const& contents);

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;

		// SScope
	public:
		CRAFT_LISP_EXPORTED virtual instance<CultSemantics> getSemantics() const override;
		CRAFT_LISP_EXPORTED virtual instance<SScope> getParentScope() const override;
		CRAFT_LISP_EXPORTED virtual size_t getSlotCount() const override;

		CRAFT_LISP_EXPORTED instance<Binding> lookup_local(instance<Symbol>) const;
		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(instance<Symbol>) const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> define(instance<Symbol> symbol, instance<BindSite> ast) override;
	};

	inline instance<SCultSemanticNode> CultSemantics::ReadState::read(instance<Sexpr> sexpr, size_t index)
	{
		return this->semantics->read_cultLisp(this, sexpr->cells[index]);
	}
	inline std::vector<instance<SCultSemanticNode>> CultSemantics::ReadState::readAll(instance<Sexpr> sexpr, size_t start_index)
	{
		std::vector<instance<SCultSemanticNode>> res;
		res.reserve(sexpr->cells.size() - start_index);
		for (auto it = sexpr->cells.begin() + start_index; it != sexpr->cells.end(); ++it)
		{
			res.push_back(this->semantics->read_cultLisp(this, *it));
		}
		return res;
	}

	/******************************************************************************
	** PSemantics for CultSemantics
	******************************************************************************/

	class CultSemanticsProvider
		: public craft::types::Implements<PSemantics>::For<CultSemantics>
	{
		virtual instance<lisp::Module> getModule(instance<> semantics) const override;

		virtual instance<> read(instance<lisp::Module> module, ReadOptions const* opts) const override;

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
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::SpecialForm);
	private:
		instance<SCultSemanticNode> _parent;

	protected:
		CRAFT_LISP_EXPORTED SpecialForm();

		friend class CultSemantics;

		CultSemantics::f_specialFormReader _read;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;
	};
    
}}
