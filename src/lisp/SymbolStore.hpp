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
		std::map<std::string_view, uint32_t> _map;

		uint32_t _anon_count;

	public:

		inline SymbolStore()
		{
			_anon_count = 0;
		}

		inline uint32_t intern(std::string const& s)
		{
			auto key = std::string_view{ s };
			auto mlb = _map.lower_bound(key);

			if (mlb != _map.end() && !(_map.key_comp()(key, mlb->first)))
				// key already exists
				return mlb->second;

			auto vit = _strings.insert(s);
			uint32_t index = (uint32_t)std::distance(_strings.begin(), vit);

			_map.insert(mlb, { std::string_view{ *vit }, index });    // Use lb as a hint to insert,

			return index;
		}

		inline size_t generate(std::string const& s_prefix = "ANON")
		{
			return intern(fmt::format("$SYM-{0}-{1}", s_prefix, ++_anon_count));
		}

		std::string const& getValue(uint32_t value) const
		{
			return *_strings.get_iterator_from_index(value);
		}
	};

}}
