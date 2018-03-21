#pragma once
#include "common.h"
#include "lisp.h"

namespace craft {
namespace lisp
{
	/* Assumes type is also PBackend
	*/

	class PSyntax
		: public craft::types::Provider
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::PSyntax, "lisp.syntax", types::SingletonProviderManager);

		//
		// Parse
		//
	public:
		struct ParseOptions
		{
			bool keep_source;
			bool no_source_locations;
			bool no_macros;
		};

		CRAFT_LISP_EXPORTED virtual instance<> parse(std::string const& s, instance<lisp::Module> into, ParseOptions const* opts) const = 0;

		//
		// Read
		//
	public:
		struct ReadOptions
		{
			bool no_macros;
		};

		CRAFT_LISP_EXPORTED virtual instance<> read(instance<> syntax, instance<lisp::Module> into, ReadOptions const* opts) const = 0;

		//
		// Introspect
		//
	public:
		struct SourceLocation
		{
			uint32_t pos;
			uint32_t length;
			uint32_t start_line;
			uint32_t start_col;
			uint32_t end_line;
			uint32_t end_col;
		};

		CRAFT_LISP_EXPORTED virtual std::string source_toString(instance<> syntax, SourceLocation const&) const = 0;

		CRAFT_LISP_EXPORTED virtual std::vector<instance<>> node_children(instance<> syntax_node) const = 0;
		CRAFT_LISP_EXPORTED virtual std::string node_toString(instance<> syntax_node) const = 0;
		CRAFT_LISP_EXPORTED virtual std::string node_toVerboseString(instance<> syntax_node) const = 0;

		CRAFT_LISP_EXPORTED virtual SourceLocation node_source(instance<> syntax_node) const = 0;
	};
}}
