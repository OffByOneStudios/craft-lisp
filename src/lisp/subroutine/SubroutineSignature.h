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
		instance<types::SType> type;

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
		instance<types::SType> returnType;

		size_t required;
		size_t singular;
		instance<Argument> variadic;
		instance<Argument> keywords;

	public:
		CRAFT_LISP_EXPORTED SubroutineSignature();

		/* The lisp type of this signature
		
		For now just the product of the argument types
		*/
		CRAFT_LISP_EXPORTED instance<types::SType> asType();

		/* Completes the signature helper data.

		Also checks for invalid signatures (throws)
		*/
		CRAFT_LISP_EXPORTED void complete();

		inline instance<SScope> scope() const { return _scope; }

		CRAFT_LISP_EXPORTED instance<SScope> read_frame(instance<SScope> const& scope);
		CRAFT_LISP_EXPORTED instance<SFrame> push_frame();
		CRAFT_LISP_EXPORTED instance<SFrame> set_frame(instance<SFrame> const& frame, std::vector<instance<>> const& args);

		/* Checks arguments to see if they are valid for this signature. (throws)
		*/
		CRAFT_LISP_EXPORTED void check(instance<Environment> env, std::vector<instance<>> const& args);

	public:

		template<typename T>
		inline static instance<types::SType> sigType()
		{
			return instance<types::CraftType>::make(cpptype<T>::typeDesc());
		}

		template<typename T>
		inline static instance<Argument> sigArgument()
		{
			auto res = instance<Argument>::make();
			res->type = sigType<T>();
			return res;
		}

		template<typename... TArgs>
		inline static instance<SubroutineSignature> makeFromArgs()
		{
			auto res = instance<SubroutineSignature>::make();
			res->arguments = { sigArgument<TArgs>()... };
			res->complete();
			return res;
		}

		template<typename... TArgs>
		inline static instance<SubroutineSignature> makeFromArgsAndReturn()
		{
			auto res = instance<SubroutineSignature>::make();
			res->arguments = { sigArgument<TArgs>()... };
			res->returnType = (res->arguments.end() - 1)->get()->type;
			res->arguments.pop_back();
			res->complete();
			return res;
		}

		inline static instance<SubroutineSignature> makeCollectArgs()
		{
			auto res = instance<SubroutineSignature>::make();

			auto arg = instance<Argument>::make();
			arg->type = instance<types::Special>::make();
			arg->type.asType<types::Special>()->kind = types::Special::Any;
			arg->flags = Argument::Flags::VariadicCollect;
			res->arguments.push_back(arg);

			res->complete();
			return res;
		}
	};
}}
