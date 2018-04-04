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
			CRAFT_LISP_EXPORTED bool truth(instance<SFrame> frame, instance<PSubroutine> truth, instance<> code);
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
			CRAFT_LISP_EXPORTED std::string symbol(instance<>);

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

			/* Takes an sexpr represting a function binding, and returns a signature

			a "macro"

			*/
			CRAFT_LISP_EXPORTED instance<SubroutineSignature> binding_expr_to_signature(instance<SScope> scope, instance<Sexpr>);
		}

		namespace system
		{
			void make_string_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_shim_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_math_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_fs_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_llvm_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_zmq_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_env_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_regex_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_http_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_meta_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_subprocess_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_list_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_map_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_platform_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_buffer_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_security_globals(instance<Module>& m, instance<Namespace>& ns);
			void make_repl_globals(instance<Module>& m, instance<Namespace>& ns);
		}
	}

	instance<Module> make_library_globals(instance<Namespace> ns);

}}

//
// C++ interfaces for libraries
//

#include "lisp/library/iteration.h"