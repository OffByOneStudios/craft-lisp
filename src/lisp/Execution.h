#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Execution
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Execution);
	private:

		instance<Environment> _environment;
		instance<Namespace> _namespace;

		std::vector<instance<SFrame>> _stack;

	public:

		CRAFT_LISP_EXPORTED Execution(instance<Environment> env, instance<Namespace> ns);

		CRAFT_LISP_EXPORTED instance<Environment> environment() const;
		CRAFT_LISP_EXPORTED instance<Namespace> namespace_() const;

		CRAFT_LISP_EXPORTED std::vector<instance<SFrame>> const& stack() const;
		CRAFT_LISP_EXPORTED void push_frame(instance<SFrame> _push);
		CRAFT_LISP_EXPORTED void pop();
	};
}}
