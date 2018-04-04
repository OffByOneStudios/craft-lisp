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
		std::string _uri;

		bool _inited;
		std::map<std::string, size_t> _lookup;
		std::vector<instance<SBinding>> _bindings;

	public:
		instance<Sexpr> content; // lisp "source" code; used by interpreter

		instance<> loader; // module specific loader
		instance<> backend; // module + backend specific object

	public:

		CRAFT_LISP_EXPORTED Module(instance<Namespace> ns, std::string uri);

		CRAFT_LISP_EXPORTED bool isLoaded() const;
		CRAFT_LISP_EXPORTED bool isInitalized() const;

		CRAFT_LISP_EXPORTED std::string uri() const;

		CRAFT_LISP_EXPORTED void setLive();

		CRAFT_LISP_EXPORTED void load(); // might be asynchronous
		CRAFT_LISP_EXPORTED void init(); // must be in order

		// Tells the module that this is the next thing to evaluate
		CRAFT_LISP_EXPORTED instance<> liveContinueWith(instance<Sexpr>);

		CRAFT_LISP_EXPORTED std::vector<instance<SBinding>> const& bindings() const;

	public:
		/* Helper functions for people writing modules in C++
		
		*/
		CRAFT_LISP_EXPORTED instance<SBinding> define_eval(std::string name, instance<> value);

	public:
		//
		// SScope
		//
		CRAFT_LISP_EXPORTED virtual instance<Environment> environment() const override;
		CRAFT_LISP_EXPORTED virtual instance<Namespace> namespace_() const override;
		inline virtual bool isDynamicScope() const override { return false; }
		CRAFT_LISP_EXPORTED virtual instance<SScope> parent() const override;
		CRAFT_LISP_EXPORTED virtual std::vector<instance<SBinding>> search(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> lookup(std::string const&) override;
		CRAFT_LISP_EXPORTED virtual instance<SBinding> define(std::string name, instance<> value) override;
	};

}}
