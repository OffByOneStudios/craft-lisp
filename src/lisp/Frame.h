#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	class Frame
		: public virtual craft::types::Object
		, public types::Implements<SFrame>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Frame);
	private:

		instance<Execution> _execution;
		instance<SFrame> _parent;
		instance<SFrame> _parent_lexical;

		instance<SScope> _scope;

		std::vector<instance<>> _values;

	public:

		CRAFT_LISP_EXPORTED Frame(instance<Execution> exe);
		CRAFT_LISP_EXPORTED Frame(instance<SFrame> parent, instance<SScope> scope);

		CRAFT_LISP_EXPORTED instance<SScope> scope() const;

		CRAFT_LISP_EXPORTED size_t push(instance<> value);
		CRAFT_LISP_EXPORTED instance<> get(size_t index);
		CRAFT_LISP_EXPORTED void set(size_t index, instance<> value);

		CRAFT_LISP_EXPORTED void setLexicalParent(instance<SFrame> value);
	public:
		//
		// SFrame
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Execution> execution() const override;

		CRAFT_LISP_EXPORTED virtual instance<SFrame> parent() const override;
		CRAFT_LISP_EXPORTED virtual instance<SFrame> lexical_parent() const override;
	};

}}
