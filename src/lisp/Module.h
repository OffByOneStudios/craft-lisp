#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Module
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Module);
	private:
		friend class Namespace;

		instance<> _value; // runtime value of initing the module

		instance<Namespace> _ns;
		std::string _uri;

		instance<> _loader; // module specific loader

		instance<> _syntax_instance;
		PSyntax* _syntax_syntax;

		struct _Semantic
		{
			instance<> instance;
			PSemantics* semantics;
		};
		std::map<types::TypeId, _Semantic> _semantics;

		instance<> _lastResult;

		// Module management
	public:

		CRAFT_LISP_EXPORTED Module(instance<Namespace> ns, std::string uri);
		CRAFT_LISP_EXPORTED void craft_setupInstance();

		CRAFT_LISP_EXPORTED instance<Namespace> getNamespace() const;

		CRAFT_LISP_EXPORTED bool isLoaded() const;
		CRAFT_LISP_EXPORTED void load();

		CRAFT_LISP_EXPORTED bool isInitialized() const;
		CRAFT_LISP_EXPORTED void initialize();

		CRAFT_LISP_EXPORTED instance<> moduleValue() const;

		CRAFT_LISP_EXPORTED instance<> lastExecutedResult() const;

		CRAFT_LISP_EXPORTED std::string uri() const;

		CRAFT_LISP_EXPORTED void appendModule(instance<lisp::Module>);
		CRAFT_LISP_EXPORTED void mergeModule(instance<lisp::Module>);

		// Language features
	public:
		CRAFT_LISP_EXPORTED instance<> get(types::TypeId type);
		CRAFT_LISP_EXPORTED instance<> require(types::TypeId type, bool force_read = true);

		CRAFT_LISP_EXPORTED instance<> exec(std::string method, types::GenericInvoke const& call = {});

		CRAFT_LISP_EXPORTED void builtin_setSemantics(instance<> semantics);

		// Templated/inline helpers
	public:
		template<typename T> // TODO, implements PSemantics or PSyntax
		inline instance<T> get()
		{ return get(cpptype<T>::typeDesc()); }
		template<typename T> // TODO, implements PSemantics or PSyntax
		inline instance<T> require()
		{ return require(cpptype<T>::typeDesc()); }

		inline bool isBuiltin() { return stdext::starts_with(uri(), std::string("builtin")); }

	};

}}
