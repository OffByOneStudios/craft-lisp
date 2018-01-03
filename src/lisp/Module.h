#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Module
		: public virtual craft::types::Object
		, public types::Implements<SScope>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Module);
	private:

		instance<Environment> _environment;
		instance<Namespace> _ns;
		std::string _filepath;

		std::map<std::string, instance<Binding>> _lookup;

	public:

		CRAFT_LISP_EXPORTED Module(instance<Namespace> ns, std::string path);

		CRAFT_LISP_EXPORTED static instance<Module> makeLiveModule(instance<Environment> env);

		CRAFT_LISP_EXPORTED std::string filepath() const;
		CRAFT_LISP_EXPORTED instance<Namespace> namespace_() const;

	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Binding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual void define(instance<Binding>) override;
	};

}}
