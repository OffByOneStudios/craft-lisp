#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class BlockBinding
		: public virtual craft::types::Object
		, public types::Implements<SBinding>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BlockBinding);
	private:
		size_t _pos;
		std::string _name;
		instance<> _expression;

		std::map<std::string, instance<>> _meta;

	public:
		CRAFT_LISP_EXPORTED BlockBinding(size_t pos, std::string name, instance<> expression = instance<>());

		CRAFT_LISP_EXPORTED size_t position() const;

		CRAFT_LISP_EXPORTED void addMeta(std::string metaKey, instance<> value);

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
