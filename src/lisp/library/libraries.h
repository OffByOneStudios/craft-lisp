#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

//
// globals.h
namespace craft {
namespace lisp {

	/*
		Namespace for special forms
	*/
	namespace special
	{
		namespace helper
		{
			bool truth(instance<Scope> scope, instance<PSubroutine> truth, instance<> code);
		}
	}

	instance<Scope> make_library_globals(instance<Environment> env);


}}

//
// C++ interfaces for libraries
//

#include "lisp/library/iteration.h"