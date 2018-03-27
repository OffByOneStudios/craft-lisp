#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"

namespace craft {
namespace lisp
{
	class Sexpr
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Sexpr)
	private:

		static std::string _toString(instance<>);
		static instance<> _fromString(std::string);

	public:

		// TODO make this an actual immutable list
		std::vector<instance<>> cells;

		std::vector<size_t> cell_locs;

	public:
		CRAFT_LISP_EXPORTED std::string toString() const;
	};
}}
