#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/library/libraries.h"

#include "lisp/semantics/cult/cult.h"
#include "lisp/backend/BootstrapInterpreter.h"
//#include "lisp/backend/llvm/llvm_internal.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library::helper;


instance<Module> library::make_module_builtin_cult_core(instance<Namespace> ns, instance<> loader)
{
	//
	// Aquire objects, set up returns
	//
	auto backend_interp = ns->get<BootstrapInterpreter>();

	auto ret = instance<Module>::make(ns, loader);
	auto sem = instance<CultSemantics>::make(ret);
	ret->builtin_setSemantics(sem);

	library::core::make_logic_globals(ret);
	library::core::make_array_globals(ret);
	library::core::make_cast_globals(ret);
	library::core::make_arithmatic_globals(ret);
	library::core::make_buffer_globals(ret);
	library::core::make_string_globals(ret);
	library::core::make_shim_globals(ret);
	library::core::make_fs_globals(ret);
	library::core::make_doc_globals(ret);
	library::core::make_env_globals(ret);
	library::core::make_regex_globals(ret);
	library::core::make_meta_globals(ret);
	library::core::make_list_globals(ret);
	library::core::make_map_globals(ret);
	library::core::make_platform_globals(ret);
	library::core::make_repl_globals(ret);
	library::core::make_json_globals(ret);

	ret->builtin_setSemantics(ret->require<CultSemantics>());
	return ret;
}
