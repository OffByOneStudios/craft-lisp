#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"
#include "lisp/syntax/syntax.h"

namespace craft {
namespace lisp
{
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
}}
