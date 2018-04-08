#pragma once
#include "hash.h"

namespace craft {
namespace lisp {
namespace library {
	class PHashable abstract
		: public types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_PROVIDER_DECLARE(PHashable, "lisp.hashable", types::SingletonProviderManager);

	public:
		virtual Hash asHash(instance<> inst) const = 0;
	};

	/******************************************************************************
	** FunctionalStringer
	******************************************************************************/

	namespace _details
	{
		static std::function<std::string(instance<>)> FunctionalHasher_defaultReport;
	}

	template <typename T>
	class FunctionalHasher
		: public types::Implements<PHashable>::For<T>
	{
		std::function<std::string(instance<>)> _stringer;

	public:
		inline FunctionalHasher(std::function<std::string(instance<T>)> const& hasher)
			: _stringer([hasher](instance<> inst) { return hasher(inst.asType<T>()); })
		{ }

		inline virtual Hash asHash(instance<> inst) const override { return _hasher(inst); }
	};
}}}


