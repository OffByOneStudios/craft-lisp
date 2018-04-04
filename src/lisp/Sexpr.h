#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

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

		CRAFT_LISP_EXPORTED virtual std::string toString() const;

		inline instance<> car() const
		{
			if (cells.size() == 0)
				return { };

			return cells.front();
		}
		inline std::vector<instance<>> cdr() const
		{
			if (cells.size() < 2)
				return { };

			return std::vector<instance<>>(cells.begin() + 1, cells.end());
		}
	};

}}