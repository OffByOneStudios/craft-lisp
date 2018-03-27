#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"

namespace craft {
namespace lisp
{

	CRAFT_LISP_EXPORTED std::vector<instance<>> parse_lisp(std::string const& text, bool add_source_locations = true);
	CRAFT_LISP_EXPORTED std::stack<instance<Sexpr>> partial_parse(std::string const& text);
}}
