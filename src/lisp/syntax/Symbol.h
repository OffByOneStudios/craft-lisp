#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"

namespace craft {
namespace lisp
{
	class Keyword;

	class Symbol
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Symbol);
	private:
		friend class Namespace;

		Symbol() = default;
		Symbol(Symbol const&) = delete;
		~Symbol() = default;

	public:

		size_t symbolStoreId;

		CRAFT_LISP_EXPORTED std::string const& Resolve() const;

		static CRAFT_LISP_EXPORTED instance<Symbol> makeSymbol(instance<Keyword> keyword);
		static CRAFT_LISP_EXPORTED instance<Symbol> makeSymbol(std::string const&);
	};

}}