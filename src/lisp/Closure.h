#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Closure
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Closure);
	private:

		PSubroutine* _provider;
		instance<> _subroutine;
		instance<SFrame> _frame;

	public:
		CRAFT_LISP_EXPORTED Closure(instance<SFrame> frame, instance<> func);

		CRAFT_LISP_EXPORTED instance<> subroutine();
		CRAFT_LISP_EXPORTED instance<SFrame> closure();

		CRAFT_LISP_EXPORTED instance<SubroutineSignature> Closure::signature();
		CRAFT_LISP_EXPORTED instance<SFrame> call_frame(instance<SFrame> parent);
		CRAFT_LISP_EXPORTED instance<> call(instance<SFrame> const& call_frame, std::vector<instance<>> const&);
	};

}}
