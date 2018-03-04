#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Namespace
		: public virtual craft::types::Object
		, public types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Namespace);
	private:
		instance<Environment> _environment;

		std::vector<instance<Module>> _module_load_list;

		std::map<std::string, instance<SBinding>> _lookup;

	public:
		instance<BootstrapInterpreter> interpreter;
		instance<PBackend> interpreter_provider;

		instance<> backend;
		instance<PBackend> backend_provider;

	public:
		CRAFT_LISP_EXPORTED Namespace(instance<Environment> env);

		CRAFT_LISP_EXPORTED instance<Module> requireModule(std::string const& s, instance<> resolver_specific_extra = instance<>());

		CRAFT_LISP_EXPORTED instance<SBinding> define(instance<SBinding>);

	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Namespace> namespace_() const override;
		inline virtual bool isDynamicScope() const override { return false; }
		CRAFT_LISP_EXPORTED virtual instance<SScope> parent() const override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> define(std::string name, instance<> value) override;
	};

}}
