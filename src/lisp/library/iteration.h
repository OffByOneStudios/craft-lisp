#pragma once

#include "SIterable.h"
#include "SIterator.h"

namespace craft {
namespace lisp {

	instance<Scope> make_library_iteration(instance<Environment> env);

}}