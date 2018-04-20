#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"

namespace craft {
namespace lisp
{

	class Keyword
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Keyword)
	private:
		friend class Namespace;

		Keyword() = default;
		Keyword(Keyword const&) = delete;
		~Keyword() = default;

	public:

		size_t symbolStoreId;

		CRAFT_LISP_EXPORTED std::string const& Resolve() const;

		static CRAFT_LISP_EXPORTED instance<Keyword> makeKeyword(std::string const&);
	};

}}