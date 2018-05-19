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
		// The requester should only be used to assist in resolution of the proto_string
		CRAFT_LISP_EXPORTED virtual std::string resolveProtoString(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra = instance<>()) const = 0;

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

		inline virtual std::string resolveProtoString(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra) const override
		{
			return T::resolve(ns, requester, proto_string, extra);
		}
		inline virtual instance<Module> loadModule(instance<Namespace> ns, std::string const& resolved_string, instance<> extra) const override
		{
			return T::load(ns, resolved_string, extra);
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
		CRAFT_LISP_EXPORTED static std::string resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra);
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& resolve_string, instance<> extra);

		CRAFT_LISP_EXPORTED instance<> getContent();
		CRAFT_LISP_EXPORTED std::string getUri();
		CRAFT_LISP_EXPORTED bool prepSemantics(instance<>);
	};

	struct GraphPropertyBuiltinModuleUri final
	{
		// is a   `char const*`
		typedef char const* value_type;
		typedef types::BasicGraphIndex<value_type> index_type;
	private:
		GraphPropertyBuiltinModuleUri() = delete;
	public:
		static constexpr types::GraphMeta::Kind craftTypes_metaKind = types::GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "cult/builtin-uri";
		inline static types::GraphPropMeta* craftTypes_metaProp_builder(types::Graph::Node* metanode)
		{
			types::graph().add<types::GraphPropertyPrinter>(metanode,
				[](types::Graph::Element* p) -> std::string { return (value_type)p->value; });
			types::graph().add<types::GraphPropertyMetaIndex>(metanode, new index_type());
			return types::GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	struct BuiltinModuleDescription
	{
	public:
		typedef void(*fn_module_initer)(instance<Module> ret);
		typedef instance<Module>(*fn_module_builder)(instance<Namespace> ns, instance<> loader);

	private:
		types::cpp::static_desc __id;

	private:
		char const* _uri;
		fn_module_builder _builder;
		fn_module_initer _initer;

		inline static void static_initer(types::cpp::DefineHelper<void> _)
		{
			types::Graph::Node* n = _;
			auto m = (BuiltinModuleDescription*)n->value;
			_.add<GraphPropertyBuiltinModuleUri>(m->uri());
		}

	public:
		BuiltinModuleDescription(char const* uri, fn_module_builder module_builder, types::cpp::_fn_register_static_init init = &static_initer)
			: __id(types::cpp::CppStaticDescKindEnum::UserInfo, this, init)
			, _uri(uri), _builder(module_builder), _initer(nullptr)
		{
			// TODO throw exception if not static time
		}
		BuiltinModuleDescription(char const* uri, fn_module_initer module_initer, types::cpp::_fn_register_static_init init = &static_initer)
			: __id(types::cpp::CppStaticDescKindEnum::UserInfo, this, init)
			, _uri(uri), _builder(nullptr), _initer(module_initer)
		{
			// TODO throw exception if not static time
		}

	public:
		inline operator types::Graph::Node*() const { return __id.node; }

		inline char const* uri() const { return _uri; }

		CRAFT_LISP_EXPORTED instance<Module> build(instance<Namespace> ns, instance<> loader);
	};

	CRAFT_LISP_EXPORTED extern BuiltinModuleDescription BuiltinCultSystem;
	CRAFT_LISP_EXPORTED extern BuiltinModuleDescription BuiltinCultCore;

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
		CRAFT_LISP_EXPORTED static std::string resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra);
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& resolve_string, instance<> extra);

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
		CRAFT_LISP_EXPORTED static std::string resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra);
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& resolve_string, instance<> extra);

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
		CRAFT_LISP_EXPORTED static std::string resolve(instance<Namespace> ns, instance<Module> requester, std::string const& proto_string, instance<> extra);
		CRAFT_LISP_EXPORTED static instance<Module> load(instance<Namespace> ns, std::string const& resolve_string, instance<> extra);

	public:
		CRAFT_LISP_EXPORTED void setModule(instance<Module> module);
		CRAFT_LISP_EXPORTED void setContent(instance<> content);

		CRAFT_LISP_EXPORTED instance<> getContent();
		CRAFT_LISP_EXPORTED std::string getUri();
		CRAFT_LISP_EXPORTED bool prepSemantics(instance<>);
	};
}}
