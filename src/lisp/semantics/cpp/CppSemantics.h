#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cpp/cpp_semantics.h"

namespace craft {
namespace lisp
{
	class CppSemantics
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::CppSemantics)
	private:
		instance<lisp::Module> _module;
		instance<> _source;

	public:
		CRAFT_LISP_EXPORTED CppSemantics(instance<lisp::Module> forModule);

		inline instance<lisp::Module> getModule() const { return _module; }
		inline instance<> getSource() const { return _source; }

	};


	/******************************************************************************
	** PSemantics for CppSemantics
	******************************************************************************/

	class CppSemanticsProvider
		: public types::Implements<PSemantics>::For<CppSemantics>
	{
		virtual instance<> read(instance<lisp::Module> module, ReadOptions const* opts) const override;

		virtual instance<> lookup(instance<> semantics, instance<Symbol>) const override;
	};

}}
