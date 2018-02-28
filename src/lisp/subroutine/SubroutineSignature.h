#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Argument
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Argument);
	public:
		std::string name;
		types::TypeId type;

		enum Flags : uint64_t
		{
			Optional = 1 << 1,
			Collect = 1 << 4,
			VariadicCollect = Collect | 1 << 5,
			KeywordCollect = Collect | 1 << 6,
		};

		Flags flags;

		instance<SBinding> binding;
	public:
		CRAFT_LISP_EXPORTED Argument();
	};

	class SubroutineSignature
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::SubroutineSignature);
	private:
		instance<SScope> _scope;

	public:
		std::vector<instance<Argument>> arguments;
		types::TypeId returnType;

		size_t required;
		size_t singular;
		instance<Argument> variadic;
		instance<Argument> keywords;

	public:
		CRAFT_LISP_EXPORTED SubroutineSignature();

		/* Completes the signature helper data.

		Also checks for invalid signatures (throws)
		*/
		CRAFT_LISP_EXPORTED void complete();

		inline instance<SScope> scope() const { return _scope; }

		CRAFT_LISP_EXPORTED instance<SScope> read_frame(instance<SScope> const& scope);
		CRAFT_LISP_EXPORTED instance<SFrame> push_frame(instance<SFrame> const& parent);
		CRAFT_LISP_EXPORTED instance<SFrame> set_frame(instance<SFrame> const& frame, std::vector<instance<>> const& args);

		/* Checks arguments to see if they are valid for this signature. (throws)
		*/
		CRAFT_LISP_EXPORTED void check(std::vector<instance<>> const& args);
	};
}}
