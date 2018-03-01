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
		typedef std::function<instance<>(instance<SFrame> frame, std::vector<instance<>> const&)> f_call;

	private:

		instance<SubroutineSignature> _signature;
		f_call _call;

	public:

		CRAFT_LISP_EXPORTED BuiltinFunction(f_call);
		CRAFT_LISP_EXPORTED BuiltinFunction(instance<SubroutineSignature>, f_call);

		CRAFT_LISP_EXPORTED instance<SubroutineSignature> signature();
		CRAFT_LISP_EXPORTED instance<SFrame> call_frame(instance<SFrame> parent);
		CRAFT_LISP_EXPORTED instance<> call(instance<SFrame> const& scope, std::vector<instance<>> const&);

		CRAFT_LISP_EXPORTED void setSignature(instance<SubroutineSignature> signature);
	};

}}
