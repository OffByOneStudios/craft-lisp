#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"

namespace craft {
namespace lisp
{
	class CultLispSyntax
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CultLispSyntax)
	private:
		instance<lisp::Module> _module;

		std::string _source;
		std::vector<instance<>> _rootForms;

	public:
		CRAFT_LISP_EXPORTED CultLispSyntax(instance<lisp::Module> forModule);

		inline instance<lisp::Module> getModule() const { return _module; }
		inline std::string const& getSource() const { return _source; }
		inline std::vector<instance<>> const& getRootForms() const { return _rootForms; }

		CRAFT_LISP_EXPORTED void parse(std::string const& s, PSyntax::ParseOptions const* opts = nullptr);

		// TODO string_view
		CRAFT_LISP_EXPORTED std::string getSource(PSyntax::SourceLocation const&) const;

		CRAFT_LISP_EXPORTED PSyntax::SourceLocation getSourceLocation(size_t const&) const;
	};


	/******************************************************************************
	** PSyntax for CultLispSyntax
	******************************************************************************/

	class CultLispSyntaxProvider
		: public types::Implements<PSyntax>::For<CultLispSyntax>
	{
		virtual instance<lisp::Module> getModule(instance<> syntax) const override;

		virtual instance<> parse(std::string const& s, instance<lisp::Module> into, ParseOptions const* opts) const override;

		virtual std::string source_toString(instance<> syntax, SourceLocation const&) const override;

		virtual std::vector<instance<>> node_children(instance<> syntax_node) const override;
		virtual std::string node_toString(instance<> syntax_node) const override;
		virtual std::string node_toVerboseString(instance<> syntax_node) const override;

		virtual SourceLocation node_source(instance<> syntax_node) const override;
	};

}}
