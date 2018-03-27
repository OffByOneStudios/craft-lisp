#pragma once
#include "lisp/common.h"
#include "lisp/lisp.h"

namespace craft {
namespace lisp
{
	class SymbolStore
	{
	private:

		// TODO, use LLVM style tries
		plf::colony<std::string> _strings;

		struct _TempStringView
		{
			std::string const& s;

			bool operator==(_TempStringView const& that) const { return s == that.s; }
			bool operator<(_TempStringView const& that) const { return s < that.s; }
			bool operator>(_TempStringView const& that) const { return s > that.s; }
		};

		typedef std::map<_TempStringView, size_t> LookupMap;
		LookupMap _map;

	public:

		inline size_t intern(std::string const& s)
		{
			auto key = _TempStringView{ s };
			auto mlb = _map.lower_bound(key);

			if (mlb != _map.end() && !(_map.key_comp()(key, mlb->first)))
			{
				// key already exists
				return mlb->second;
			}

			auto vit = _strings.insert(s);
			size_t index = std::distance(_strings.begin(), vit);

			_map.insert(mlb, LookupMap::value_type(_TempStringView{ *vit }, index));    // Use lb as a hint to insert,

			return index;
		}

		std::string const& getValue(size_t value) const
		{
			return *_strings.get_iterator_from_index(value);
		}
	};

}}
