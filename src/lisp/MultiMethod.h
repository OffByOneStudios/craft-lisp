#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class MultiMethod
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::MultiMethod);
	private:

		// TODO dispatch tree
		instance<> _only;

	public:
		CRAFT_LISP_EXPORTED MultiMethod();

		CRAFT_LISP_EXPORTED instance<> call(instance<Scope> const& scope, std::vector<instance<>> const&);

		CRAFT_LISP_EXPORTED instance<> dispatch(/* nothing at the moment */);
		CRAFT_LISP_EXPORTED void attach(instance<>);
	};

}}
