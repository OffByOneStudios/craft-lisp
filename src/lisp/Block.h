#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Block
		: public virtual craft::types::Object
		, public types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Block);
	private:

		instance<Environment> _environment;
		instance<SScope> _parent;

		std::vector<instance<BlockBinding>> _bindings;
		std::map<std::string, size_t> _lookup;

	public:
		CRAFT_LISP_EXPORTED Block(instance<SScope> parent);


	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Namespace> namespace_() const override;
		CRAFT_LISP_EXPORTED virtual instance<SScope> parent() const override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> define(std::string name, instance<> value) override;
	};
}}
