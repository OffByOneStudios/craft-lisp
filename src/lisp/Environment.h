#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Environment
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Environment);
	private:

		std::shared_ptr<spdlog::logger> _logger;

		// abstract type hierarchy
		std::map<types::TagId, types::TagId> _type_tagParents;
		std::map<types::TypeId, types::TagId> _type_typeParents;

	public:

		instance<Scope> global;

	public:

		CRAFT_LISP_EXPORTED Environment(std::shared_ptr<spdlog::logger> logger);

		// Global Logger
		CRAFT_LISP_EXPORTED std::shared_ptr<spdlog::logger> log();

		// Parser Access
		CRAFT_LISP_EXPORTED instance<Sexpr> read(std::string const& text);

		// Interpreter Access
		CRAFT_LISP_EXPORTED instance<> eval(instance<>, instance<Scope>);

		// Type System
		CRAFT_LISP_EXPORTED bool type_isChild(instance<>, types::TagId);
		CRAFT_LISP_EXPORTED bool type_isChild(types::TypeId, types::TagId);
		CRAFT_LISP_EXPORTED bool type_isChild(types::TagId, types::TagId);

		CRAFT_LISP_EXPORTED types::TagId type_getParent(instance<>);
		CRAFT_LISP_EXPORTED types::TagId type_getParent(types::TypeId);
		CRAFT_LISP_EXPORTED types::TagId type_getParent(types::TagId);

		CRAFT_LISP_EXPORTED void type_setParent(instance<>, types::TagId);
		CRAFT_LISP_EXPORTED void type_setParent(types::TypeId, types::TagId);
		CRAFT_LISP_EXPORTED void type_setParent(types::TagId, types::TagId);

		CRAFT_LISP_EXPORTED std::string type_name(types::TagId tag);
	};

}}
