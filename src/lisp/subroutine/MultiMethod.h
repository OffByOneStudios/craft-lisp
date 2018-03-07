#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/*
		This implementes the multimethod dispatch features that are core to the extensibility of 
		the lisp language.

		We implement our own type system objects here for constructing dispatch arguments. The type
		system does not yet provide all of the features we require...

		TODO implement using value equality if those are given as our dispatch types
	*/

	class MultiMethod
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::MultiMethod);
	private:

		instance<SubroutineSignature> _signature;

		struct _Dispatch
		{
			instance<types::SType> type;
			size_t index;
		};

		std::vector<instance<>> _subMethods;
		std::vector<_Dispatch> _dispatchChain;

	public:
		CRAFT_LISP_EXPORTED MultiMethod();

		CRAFT_LISP_EXPORTED instance<SubroutineSignature> signature();
		CRAFT_LISP_EXPORTED instance<SFrame> call_frame();
		CRAFT_LISP_EXPORTED instance<> call(instance<SFrame> const& frame, std::vector<instance<>> const&);

		CRAFT_LISP_EXPORTED size_t subroutineCount();

		// Provides a configuration and results working memory
		struct Dispatch
		{
			enum class Config
			{
				Default = 0,
				FastFail = 1,
			};

			Config config;

			instance<> subroutine;
			std::vector<instance<>> possible_subroutine;

			Dispatch()
			{
				config = Config::FastFail;
			}
		};

		CRAFT_LISP_EXPORTED void dispatch(instance<Environment> env, instance<types::SType> type, Dispatch* dispatch);
		CRAFT_LISP_EXPORTED void attach(instance<Environment> env, instance<>);
	};

}}
