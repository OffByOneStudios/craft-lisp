#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class BuiltinFunction final
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BuiltinFunction);
	public:
		typedef std::function<instance<>(instance<SScope> scope, std::vector<instance<>> const&)> f_call;

	private:

		f_call _call;

	public:

		CRAFT_LISP_EXPORTED BuiltinFunction(f_call);

		CRAFT_LISP_EXPORTED instance<> call(instance<SScope> const& scope, std::vector<instance<>> const&);
	};

}}
