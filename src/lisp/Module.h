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
	public:

		enum class State : uint32_t
		{
			None = 0,
			FLAG_HasLoader = 1 << 1,
			FLAG_IsLoaded = 1 << 2,
			FLAG_IsInitializing = 1 << 3,
			FLAG_WasInitialized = 1 << 4,
			FLAG_IsInitialized = 1 << 5,

			// State Transistions:
			// None -> HasLoader		// constructor();
			// HasLoader -> Loaded		// load();
			// Loaded -> Initializing	// initialize() begin;
			// Initializing -> Ready	// initialize(); update();
			// Ready -> ReInitializing  // update();

			HasLoader = None | FLAG_HasLoader,
			Loaded = None | FLAG_HasLoader | FLAG_IsLoaded,
			Initializing = None | FLAG_HasLoader | FLAG_IsLoaded | FLAG_IsInitializing,
			ReInitializing = None | FLAG_HasLoader | FLAG_IsLoaded | FLAG_IsInitializing | FLAG_WasInitialized,
			Ready = None | FLAG_HasLoader | FLAG_IsLoaded | FLAG_WasInitialized | FLAG_IsInitialized,
		};

	private:
		friend class BootstrapInterpreter;

		instance<> _value; // runtime value of initing the module, store this somewhere better

		std::mutex _mutex;
		State _state;

		instance<Namespace> _ns;
		std::string _uri;

		instance<> _loader; // module specific loader

		instance<> _syntax_instance;
		PSyntax* _syntax_syntax;

		struct _Semantic
		{
			instance<> inst;
			PSemantics* semantics;
		};
		std::map<types::TypeId, _Semantic> _semantics;

		instance<> _lastResult;

		// Module management
	public:

		CRAFT_LISP_EXPORTED Module(instance<Namespace> ns, instance<> loader);
		CRAFT_LISP_EXPORTED void craft_setupInstance();

		CRAFT_LISP_EXPORTED instance<Namespace> getNamespace() const;
		CRAFT_LISP_EXPORTED instance<> getLoader() const;

		CRAFT_LISP_EXPORTED State getState() const;

		CRAFT_LISP_EXPORTED bool hasLoader() const;
		CRAFT_LISP_EXPORTED bool isLoaded() const;
		CRAFT_LISP_EXPORTED bool isInitializing() const;
		CRAFT_LISP_EXPORTED bool wasInitialized() const;
		CRAFT_LISP_EXPORTED bool isReady() const;


		// Returns true if the load is completed (may be in progress; exceptions on errors)
		CRAFT_LISP_EXPORTED bool load();
		// Returns true if the initialize is completed (may be in progress; exceptions on errors)
		CRAFT_LISP_EXPORTED bool initialize();
		// Returns true if the update is completed (may be in progress; exceptions on errors)
		CRAFT_LISP_EXPORTED bool update(bool force = false);

		CRAFT_LISP_EXPORTED instance<> moduleValue() const;

		CRAFT_LISP_EXPORTED instance<> lastExecutedResult() const;

		CRAFT_LISP_EXPORTED std::string uri() const;

		CRAFT_LISP_EXPORTED void appendModule(instance<lisp::Module>);
		CRAFT_LISP_EXPORTED void mergeModule(instance<lisp::Module>);

		// Language features
	public:
		CRAFT_LISP_EXPORTED instance<> get(types::TypeId type);
		CRAFT_LISP_EXPORTED instance<> require(types::TypeId type);

		CRAFT_LISP_EXPORTED instance<> eval(std::string const& code);
		CRAFT_LISP_EXPORTED instance<> exec(std::string method, types::GenericInvoke const& call = {});

		CRAFT_LISP_EXPORTED void builtin_setSemantics(instance<> semantics);

		// Templated/inline helpers
	public:
		template<typename T> // TODO, implements PSemantics or PSyntax
		inline instance<T> get()
		{ return get(types::cpptype<T>::typeDesc()); }
		template<typename T> // TODO, implements PSemantics or PSyntax
		inline instance<T> require()
		{ return require(types::cpptype<T>::typeDesc()); }

		inline bool isBuiltin() { return stdext::starts_with(uri(), std::string("builtin")); }

	};

	inline bool operator& (Module::State a, Module::State b) { return ((uint32_t)a & (uint32_t)b) != 0; }
}}
