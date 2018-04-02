#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Binding
		: public virtual craft::types::Object
		, public types::Implements<SBinding>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Binding);
	private:
		std::string _name;
		instance<> _expression;
		instance<> _value;

		std::map<std::string, instance<>> _meta;

	public:
		CRAFT_LISP_EXPORTED Binding(std::string name, instance<> expression);

		CRAFT_LISP_EXPORTED instance<> expression();
		CRAFT_LISP_EXPORTED void setValue(instance<> value);
		CRAFT_LISP_EXPORTED instance<> value();

		CRAFT_LISP_EXPORTED void addMeta(std::string metaKey, instance<> value) override;

		//
		// SBinding
		//
	public:
		CRAFT_LISP_EXPORTED virtual std::string name() const override;
		CRAFT_LISP_EXPORTED virtual instance<> getValue(instance<SFrame> frame) const override;
		CRAFT_LISP_EXPORTED virtual instance<> getMeta(std::string metaKey, types::TypeId type = types::None) override;

		CRAFT_LISP_EXPORTED virtual instance<> eval(instance<SFrame> frame) override;
	};

}}
