#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** PModuleLoader
	******************************************************************************/

	class PModuleLoader
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PModuleLoader, "lisp.module-loader", types::NamedSingletonProviderManager);

	public:
		// Creates a new backend for the given namespace
		CRAFT_LISP_EXPORTED virtual instance<Module> loadModule(instance<Namespace> ns, std::string const& proto_string, instance<> extra = instance<>()) const = 0;

		// Retrieves content from the loader
		CRAFT_LISP_EXPORTED virtual instance<> getContent(instance<> loader) const = 0;
		CRAFT_LISP_EXPORTED virtual std::string getUri(instance<> loader) const = 0;

		// Retrieves content from the loader
		CRAFT_LISP_EXPORTED virtual bool prepSemantics(instance<> loader, instance<> semantics) const = 0;

		CRAFT_LISP_EXPORTED virtual std::string provider_index() const = 0;
	};

	template<typename T>
	class AutoModuleLoader final
		: public types::Implements<PModuleLoader>::For<T>
	{
	public:
		inline virtual instance<Module> loadModule(instance<Namespace> ns, std::string const& proto_string, instance<> extra) const override
		{
			return typename T::load(ns, proto_string, extra);
		}

		inline virtual instance<> getContent(instance<> loader) const override
		{
			return loader.asType<T>()->getContent();
		}
		inline virtual std::string getUri(instance<> loader) const override
		{
			return loader.asType<T>()->getUri();
		}

		inline virtual bool prepSemantics(instance<> loader, instance<> semantics) const override
		{
			return loader.asType<T>()->prepSemantics(semantics);
		}

		inline virtual std::string provider_index() const override
		{
			return T::c_provider_index;
		}
	};

	/******************************************************************************
	** BuiltinLoader
	******************************************************************************/

	class BuiltinLoader
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::BuiltinLoader);

	public:
		static constexpr char c_provider_index[] = "builtin";

	private:
		instance<Module> _module;
		std::string _builtinName;

	private:
		BuiltinLoader() = default;

	public:
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& proto_string, instance<> extra);

		CRAFT_LISP_EXPORTED instance<> getContent();
		CRAFT_LISP_EXPORTED std::string getUri();
		CRAFT_LISP_EXPORTED bool prepSemantics(instance<>);
	};

	/******************************************************************************
	** FileLoader
	******************************************************************************/

	class FileLoader
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::FileLoader);

	public:
		static constexpr char c_provider_index[] = "file";

	private:
		instance<Module> _module;
		std::string _filePath;

	private:
		FileLoader() = default;

	public:
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& proto_string, instance<> extra);

		CRAFT_LISP_EXPORTED instance<> getContent();
		CRAFT_LISP_EXPORTED std::string getUri();
		CRAFT_LISP_EXPORTED bool prepSemantics(instance<>);
	};

	/******************************************************************************
	** ReplLoader
	******************************************************************************/

	class ReplLoader
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::ReplLoader);

	public:
		static constexpr char c_provider_index[] = "repl";

	private:
		instance<Module> _module;
		std::string _replName;

	private:
		ReplLoader() = default;

	public:
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& proto_string, instance<> extra);

		CRAFT_LISP_EXPORTED instance<> getContent();
		CRAFT_LISP_EXPORTED std::string getUri();
		CRAFT_LISP_EXPORTED bool prepSemantics(instance<>);
	};

	/******************************************************************************
	** AnonLoader
	******************************************************************************/

	class AnonLoader
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::AnonLoader);

	public:
		static constexpr char c_provider_index[] = "anon";

	private:
		instance<Module> _module;
		std::string _anonName;

		instance<Module> _asModule;
		instance<> _content;

	public:
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& proto_string, instance<> extra);

	public:
		CRAFT_LISP_EXPORTED void setModule(instance<Module> module);
		CRAFT_LISP_EXPORTED void setContent(instance<> content);

		CRAFT_LISP_EXPORTED instance<> getContent();
		CRAFT_LISP_EXPORTED std::string getUri();
		CRAFT_LISP_EXPORTED bool prepSemantics(instance<>);
	};
}}
