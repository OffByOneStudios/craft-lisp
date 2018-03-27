#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult_semantics.h"

namespace craft {
namespace lisp
{
	class CultSemantics
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CultSemantics)
	private:
		instance<lisp::Module> _module;
		instance<> _source;

	public:
		CRAFT_LISP_EXPORTED CultSemantics(instance<lisp::Module> forModule);

		inline instance<lisp::Module> getModule() const { return _module; }
		inline instance<> getSource() const { return _source; }

	};


	/******************************************************************************
	** PSyntax for CultLispSyntax
	******************************************************************************/

	class CultLispSyntaxProvider
		: public types::Implements<PSemantics>::For<CultSemantics>
	{
		virtual std::vector<TypeId> readsFrom() const override;
		virtual instance<> read(instance<> syntax, instance<lisp::Module> into, ReadOptions const* opts) const override;

		virtual std::vector<TypeId> transformsFrom() const override;
		virtual instance<> transform(instance<> semantics, instance<lisp::Module> into, instance<> transformationOptions = instance<>()) const override;

		virtual instance<> lookup(instance<> semantics, std::string const&) const override;
	};

}}
