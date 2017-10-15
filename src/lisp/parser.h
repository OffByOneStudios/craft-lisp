#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{

	CRAFT_LISP_EXPORTED instance<Sexpr> parse_lisp(std::string const& text);

}}
