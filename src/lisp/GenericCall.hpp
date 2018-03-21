#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{
	/* Helper structure for containing a generic call from the bootstrap implementation
	*/

	struct GenericCall
	{
		std::vector<instance<>> args;

		inline GenericCall(std::initializer_list<instance<>> l)
		{
			args.insert(args.end(), l.begin(), l.end());
		}
	};
}}
