#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Environment
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(Environment);
	private:

	public:

		instance<Scope> global;

	public:

		CRAFT_LISP_EXPORTED Environment();

		CRAFT_LISP_EXPORTED instance<Sexpr> read(std::string const& text);
		CRAFT_LISP_EXPORTED instance<> eval(instance<>, instance<Scope>);
	};

}}
