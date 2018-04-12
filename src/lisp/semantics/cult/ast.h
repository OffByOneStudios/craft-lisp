#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/semantics/cult/cult.h"

namespace craft {
namespace lisp
{
	/******************************************************************************
	** SCultSemanticNode
	******************************************************************************/

	/*
		Represents a cult semantic node.

		Should also provide:
		* Cloneable
	*/
	class SCultSemanticNode
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::SCultSemanticNode, "lisp.cult.semantic", types::FactoryAspectManager);

	public:
		struct ast_error : stdext::exception
		{
			using exception::exception;
		};

		struct parent_already_set_error : ast_error
		{
			parent_already_set_error(instance<SCultSemanticNode> node)
				: ast_error("Node `{0}` has alreayd has parent set.", node.toString())
			{ }
		};

		struct ValidationState
		{
			struct Entry
			{
				instance<SCultSemanticNode> node;
				std::string message;
				spdlog::level::level_enum level;
			};

			std::vector<Entry> entries;
			spdlog::level::level_enum worst_level;

			inline void add(instance<SCultSemanticNode> node, spdlog::level::level_enum level, std::string message)
			{
				if (level > worst_level)
					worst_level = level;

				entries.push_back({ node, message, level });
			}

			template<typename ...TArgs>
			inline void error(instance<SCultSemanticNode> node, TArgs &&... args)
			{
				add(node, spdlog::level::level_enum::err, fmt::format(std::forward<TArgs>(args)...));
			}

			template<typename ...TArgs>
			inline void warning(instance<SCultSemanticNode> node, TArgs &&... args)
			{
				add(node, spdlog::level::level_enum::warn, fmt::format(std::forward<TArgs>(args)...));
			}

			template<typename ...TArgs>
			inline void info(instance<SCultSemanticNode> node, TArgs &&... args)
			{
				add(node, spdlog::level::level_enum::info, fmt::format(std::forward<TArgs>(args)...));
			}
		};

		CRAFT_LISP_EXPORTED virtual void validate(ValidationState*) const;

		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const = 0;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) = 0;

	protected:
		inline static instance<SCultSemanticNode> _ast(instance<SCultSemanticNode> parent, instance<SCultSemanticNode> child)
		{
			child->setParent(parent);
			return child;
		}
	};

	/******************************************************************************
	** Constant
	******************************************************************************/

	/*
		Represents an "as parsed from ast" constant.
	*/
	class Constant
		: public virtual craft::types::Object
		, public craft::types::Implements<SCultSemanticNode>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Constant);
	private:
		instance<SCultSemanticNode> _parent;

	private:
		instance<> _value;

	public:
		CRAFT_LISP_EXPORTED Constant(instance<> value);

		CRAFT_LISP_EXPORTED instance<> getValue() const;

		// SCultSemanticNode
	public:
		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const override;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>) override;
	};
}}
