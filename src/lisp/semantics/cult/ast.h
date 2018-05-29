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

	protected:
		friend class CultSemantics;

		instance<SCultSemanticNode> _parent;
		size_t _source_start, _source_end;

	public:
		struct ast_error : stdext::exception
		{
			using exception::exception;
		};

		struct parent_already_set_error : ast_error
		{
			parent_already_set_error(instance<SCultSemanticNode> node)
				: ast_error("Node `{0}` has already has parent set.", node.toString())
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

		CRAFT_LISP_EXPORTED virtual instance<SCultSemanticNode> getParent() const;
		CRAFT_LISP_EXPORTED virtual void setParent(instance<SCultSemanticNode>);

		// Binds (or re-binds) each member of the tree. Call your children.
		CRAFT_LISP_EXPORTED virtual void bind();

		CRAFT_LISP_EXPORTED virtual void validate(ValidationState*) const;

		CRAFT_LISP_EXPORTED std::string sourceLocationToString() const;

	protected:

		inline instance<SCultSemanticNode> _ast(instance<SCultSemanticNode> child)
		{
			child->setParent(craft_featuredInstance());
			return child;
		}

		inline static instance<SCultSemanticNode> _clone(instance<SCultSemanticNode> to_clone)
		{
			instance<SCultSemanticNode> res = to_clone.getFeature<types::PClone>()->clone(to_clone);
			res->_source_start = to_clone->_source_start;
			res->_source_end = to_clone->_source_end;

			return res;
		}

		inline instance<SCultSemanticNode> _astclone(instance<SCultSemanticNode> to_clone)
		{
			if (!to_clone) return to_clone;
			return _ast(_clone(to_clone));
		}
		inline instance<SCultSemanticNode> _astbind(instance<SCultSemanticNode> to_bind)
		{
			auto ret = _ast(to_bind);
			ret->bind();
			return ret;
		}
		inline instance<SCultSemanticNode> _astclonebind(instance<SCultSemanticNode> to_clonebind)
		{
			auto ret = _astclone(to_clonebind);
			ret->bind();
			return ret;
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
		instance<> _value;

	public:
		CRAFT_LISP_EXPORTED Constant(instance<> value);

		CRAFT_LISP_EXPORTED instance<> getValue() const;
	};
}}
