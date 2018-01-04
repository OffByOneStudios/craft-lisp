#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Binding
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Binding);
	private:
		std::string _name;
		instance<> _value;

		std::map<std::string, instance<>> _meta;

	public:
		CRAFT_LISP_EXPORTED Binding(std::string name, instance<> value);

		CRAFT_LISP_EXPORTED instance<> eval(instance<SScope> scope);

		inline std::string name() const { return _name; }
		inline instance<> value() const { return _value; }

		CRAFT_LISP_EXPORTED void addMeta(std::string metaKey, instance<> value);
		CRAFT_LISP_EXPORTED instance<> getMeta(std::string metaKey, types::TypeId type = types::None);

		template<typename T>
		inline instance<T> getMeta(std::string metaKey) { return getMeta(metaKey, types::type<T>::typeId()).asType<T>(); }
	};

}}
