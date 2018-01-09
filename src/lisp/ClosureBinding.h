#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class ClosureBinding
		: public virtual craft::types::Object
		, public types::Implements<SBinding>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::ClosureBinding);
	private:
		instance<BlockBinding> _base;
		size_t _lexical_pos;

	public:
		CRAFT_LISP_EXPORTED ClosureBinding(instance<BlockBinding> base, size_t lexical_pos = 0);

		CRAFT_LISP_EXPORTED size_t positionLexical() const;
		CRAFT_LISP_EXPORTED size_t positionFrame() const;

		CRAFT_LISP_EXPORTED void incrementClosure();
		CRAFT_LISP_EXPORTED void setClosure(size_t lexical_pos);

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
