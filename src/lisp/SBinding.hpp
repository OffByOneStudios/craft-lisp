#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class SBinding
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_ASPECT_DECLARE(craft::lisp::SBinding, "lisp.binding", types::FactoryAspectManager);

	public:
		CRAFT_LISP_EXPORTED virtual std::string name() const = 0;
		CRAFT_LISP_EXPORTED virtual instance<> getValue(instance<SFrame> frame) const = 0;
		CRAFT_LISP_EXPORTED virtual instance<> getMeta(std::string metaKey, types::TypeId type = types::None) = 0;

		CRAFT_LISP_EXPORTED virtual instance<> eval(instance<SFrame> frame) = 0;

		template<typename T>
		inline instance<T> getMeta(std::string metaKey) { return getMeta(metaKey, types::type<T>::typeId()).asType<T>(); }
	};

}}
