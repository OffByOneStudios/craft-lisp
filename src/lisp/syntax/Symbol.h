#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	// TODO move to root
	class Symbol
		: public virtual craft::types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::Symbol);
	public:
		enum class Path : uint32_t
		{
			None = 1 << 24,
			Slash = None | (1 << 25),
			Colon = None | (1 << 26),
			Dot = None | (1 << 27),
			At = None | (1 << 28),
		};

		static inline bool isPath(uint32_t s)
		{
			return (s & (uint32_t)Path::None) != 0;
		}

		static inline char toChar(uint32_t s)
		{
			switch ((Path)s)
			{
			case Path::Slash: return '/';
			case Path::Colon: return ':';
			case Path::Dot: return '.';
			case Path::At: return '@';
			default: return '\0';
			}
		}

	private:
		Symbol() = default;
		Symbol(Symbol const&) = default;
		~Symbol() = default;

		std::vector<uint32_t> _symbolseq;

	public:

		CRAFT_LISP_EXPORTED size_t size() const;
		CRAFT_LISP_EXPORTED uint32_t at(size_t) const;

		CRAFT_LISP_EXPORTED bool isKeyword() const;
		CRAFT_LISP_EXPORTED bool isSimple() const;

		CRAFT_LISP_EXPORTED std::string getDisplay() const;

		static CRAFT_LISP_EXPORTED instance<Symbol> makeSymbol(std::vector<uint32_t> const&);
		static CRAFT_LISP_EXPORTED instance<Symbol> makeSymbol(std::string const&);

		CRAFT_LISP_EXPORTED instance<Symbol> trimKeyword() const;


		inline bool operator==(Symbol const& other) const {
			if (_symbolseq.size() != other._symbolseq.size()) return false;
			for (auto i = 0; i < _symbolseq.size(); i++)
			{
				if (_symbolseq[i] != other._symbolseq[i]) return false;
			}

			return true;
		}

		inline bool operator<(Symbol const& other) const {
			if (other._symbolseq.size() > _symbolseq.size()) return false;
			else if (other._symbolseq.size() < _symbolseq.size()) return true;
			for (auto i = 0; i < _symbolseq.size(); i++)
			{
				if (_symbolseq[i] < other._symbolseq[i]) return true;
			}

			return false;

		}

		inline bool operator>(Symbol const& other) const {
			if (other._symbolseq.size() > _symbolseq.size()) return true;
			else if (other._symbolseq.size() < _symbolseq.size()) return false;
			for (auto i = 0; i < _symbolseq.size(); i++)
			{
				if (_symbolseq[i] > other._symbolseq[i]) return true;
			}

			return false;

		}

		bool operator <=(Symbol const& other) { return this->operator<(other) || this->operator==(other); }
		bool operator !=(Symbol const& other) { return !this->operator==(other); }
		bool operator >=(Symbol const& other) { return !this->operator<(other) || this->operator==(other); }
	};

	// TODO move to a new file:
	template <typename T>
	struct SymbolTableIndexed
	{
		std::map<size_t, size_t> table;
		std::vector<T> bindings;

		inline T lookup(instance<Symbol> symbol) const
		{
			if (!symbol->isSimple())
				return T();

			auto it = table.find(symbol->at(0));
			if (it == table.end())
				return T();
			return bindings[it->second];
		}
		template <class F>
		inline T define(instance<Symbol> symbol, F&& builder)
		{
			if (!symbol->isSimple())
				throw stdext::exception("Symbol is not simple.");

			auto key = symbol->at(0);
			auto lb = table.lower_bound(key);

			if (lb != table.end() && !(table.key_comp()(key, lb->first)))
				throw stdext::exception("Symbol already defined.");

			size_t index = bindings.size();
			T res = builder(index);

			bindings.push_back(res);
			table.insert(lb, { key, index });

			return res;
		}
	};
}}
