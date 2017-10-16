#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp {

	instance<Scope> make_library_globals(instance<Environment> env);

}}
