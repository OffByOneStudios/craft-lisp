#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/*
		This implementes the multimethod dispatch features that are core to the extensibility of 
		the lisp language.

		We implement our own type system objects here for constructing dispatch arguments. The type
		system does not yet provide all of the features we require...
	*/

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

		CRAFT_LISP_EXPORTED instance<> dispatch(instance<Environment> env);
		CRAFT_LISP_EXPORTED void attach(instance<Environment> env, instance<>);
	};

}}
