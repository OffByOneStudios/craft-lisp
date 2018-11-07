#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class Environment
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Environment);
	private:
		std::shared_ptr<spdlog::logger> _logger;
		std::vector<instance<std::string>> _argv;

		std::recursive_mutex _module_load_mutex;
		std::map<std::string, size_t> _module_cache;
		std::vector<instance<Module>> _module_load_list;

		struct _Backend
		{
			instance<> inst;
			PBackend* backend;
			PExecutor* executor;
			PCompiler* compiler;
		};

		std::map<types::TypeId, _Backend> _backends;

		size_t _loaderVar_anonCount;

		CRAFT_LISP_EXPORTED _Backend preferedBackend() const;
		CRAFT_LISP_EXPORTED _Backend fallbackBackend() const;

		// Public components
	public:
		SymbolStore symbolStore;

		Signal<void(instance<Module>)> on_moduleInit;

		// Setup
	public:
		CRAFT_LISP_EXPORTED Environment(std::shared_ptr<spdlog::logger> logger, std::vector<instance<std::string>> argv = {});
		CRAFT_LISP_EXPORTED void craft_setupInstance();

		// Globals
	public:
	
		// Global Logger
		CRAFT_LISP_EXPORTED std::shared_ptr<spdlog::logger> log();
		// Global Argv
		CRAFT_LISP_EXPORTED std::vector<instance<std::string>> argv();

		// Language features
	public:
		CRAFT_LISP_EXPORTED instance<> get(types::TypeId type);

		CRAFT_LISP_EXPORTED instance<> parse(std::string contents, types::TypeId type, PSyntax::ParseOptions const* opts = nullptr);
		
		CRAFT_LISP_EXPORTED instance<> read(std::string contents, types::TypeId parseType, types::TypeId readType, PSyntax::ParseOptions const* popts = nullptr, PSemantics::ReadOptions const* ropts = nullptr);
		CRAFT_LISP_EXPORTED instance<> read(instance<> source, types::TypeId type, PSemantics::ReadOptions const* opts = nullptr);

		CRAFT_LISP_EXPORTED instance<> exec(instance<Module> module, std::string method, types::GenericInvoke const& call = {});

		CRAFT_LISP_EXPORTED void compile(std::string path, instance<> compiler_options);
		CRAFT_LISP_EXPORTED void compile(instance<Module> module, std::string path, instance<> compiler_options);

		// Core features
	public:
		CRAFT_LISP_EXPORTED instance<Module> importModule(instance<Module> requestingModule, std::string const& s, instance<> resolver_specific_extra = instance<>());

		// Misc features
	public:
		CRAFT_LISP_EXPORTED void refreshBackends();

		// Templated helpers
	public:
		
		template<typename T> // TODO, implements PBackend
		inline instance<T> get()
		{ return get(types::cpptype<T>::typeDesc()); }

		template<typename T>
		inline instance<T> parse(std::string contents, PSyntax::ParseOptions const* opts = nullptr)
		{ return parse(contents, types::cpptype<T>::typeDesc(), opts); }

		template<typename T>
		inline instance<T> read(std::string contents, PSemantics::ReadOptions const* opts = nullptr)
		{ return read(contents, types::cpptype<T>::typeDesc(), opts); }
		template<typename T>
		inline instance<T> read(instance<> source, PSemantics::ReadOptions const* opts = nullptr)
		{ return read(source, types::cpptype<T>::typeDesc(), opts); }
	};

}}
