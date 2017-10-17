#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Scope
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Scope);
	private:

		instance<Environment> _environment;
		instance<Scope> _parent;

		std::map<std::string, instance<>> _namespace;

	public:

		CRAFT_LISP_EXPORTED Scope(instance<Environment> env, instance<Scope> parent);

		CRAFT_LISP_EXPORTED instance<Environment> environment() const;
		CRAFT_LISP_EXPORTED instance<Scope> parent() const;

		CRAFT_LISP_EXPORTED instance<> lookup(std::string const&);
		CRAFT_LISP_EXPORTED void def(std::string const&, instance<>);
	};

}}