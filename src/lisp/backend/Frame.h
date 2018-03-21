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

		instance<> _representative;
		instance<SScope> _scope;

		instance<SFrame> _parent_lexical;

		instance<Execution> _execution;

		size_t _stack_location;


		std::vector<instance<>> _values;

	public:

		CRAFT_LISP_EXPORTED Frame();
		CRAFT_LISP_EXPORTED Frame(instance<SScope> scope);

		CRAFT_LISP_EXPORTED instance<SScope> getScope() const;
		CRAFT_LISP_EXPORTED instance<> getRepresentative() const;

		CRAFT_LISP_EXPORTED size_t push(instance<> value);
		CRAFT_LISP_EXPORTED instance<> get(size_t index);
		CRAFT_LISP_EXPORTED void set(size_t index, instance<> value);

		CRAFT_LISP_EXPORTED void setLexicalParent(instance<SFrame> value);
		CRAFT_LISP_EXPORTED void setRepresentative(instance<>);
	public:
		//
		// SFrame
		//
		CRAFT_LISP_EXPORTED virtual instance<Namespace> getNamespace() const override;
		CRAFT_LISP_EXPORTED virtual instance<Execution> getExecution() const override;

		CRAFT_LISP_EXPORTED virtual instance<SFrame> getPrevious() const override;
		CRAFT_LISP_EXPORTED virtual instance<SFrame> getLexicalParent() const override;

		CRAFT_LISP_EXPORTED void beginExecution(instance<Execution> value) override;
	};

}}
