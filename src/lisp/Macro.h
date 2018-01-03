#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Macro final
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Macro);
	public:
		typedef std::function<instance<>(instance<SScope>, std::vector<instance<>> const&)> f_macro;

	private:

		f_macro _macro;

	public:

		CRAFT_LISP_EXPORTED Macro(f_macro);

		CRAFT_LISP_EXPORTED instance<> expand(instance<SScope> const& scope, std::vector<instance<>> const& forms);
	};

}}
