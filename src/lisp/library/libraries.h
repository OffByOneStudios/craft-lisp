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
			bool truth(instance<SScope> scope, instance<PSubroutine> truth, instance<> code);
		}
	}

	/*
	Namespace for special forms
	*/
	namespace library
	{
		namespace helper
		{
			/* Try to get a string from a symbol like object (e.g. strings, symbols, keywords).
			
			*/
			std::string symbol(instance<>);

			/* Throw an exception if a given argument is not of the expected type

			*/
			template <typename T>
			inline instance<T> expect(instance<> inst)
			{
				if (!inst)
					return inst;

				if (inst.typeId().isType<T>())
					return inst;

				throw stdext::exception("Expected {0} got {1}", types::type<T>::typeId().toString(), inst.typeId().toString());
			}
		}
	}

	instance<Module> make_library_globals(instance<Namespace> ns);


}}

//
// C++ interfaces for libraries
//

#include "lisp/library/iteration.h"