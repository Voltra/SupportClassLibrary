#pragma once

#include <unordered_map>
#include <unordered_set>

namespace scl{
	/**
	 * @namespace scl::alias
	 * Namespace for all the type aliases
	 */
	namespace alias{
		/**
		 * A collection of key-value pairs
		 * @tparam Key being the type of the keys
		 * @tparam Value being the type of the values
		 */
		template <class Key, class Value, class... Ts>
		using Map = std::unordered_map<Key, Value, Ts...>;

		/**
		 * A set of unique values
		 * @tparam Value being the type of values stored
		 */
		template <class Value, class... Ts>
		using Set = std::unordered_set<Value, Ts...>;
	}
}