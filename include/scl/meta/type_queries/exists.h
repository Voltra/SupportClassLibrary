#pragma once
#include "../type_aliases/void_t.h"

namespace scl {
    namespace meta {
	namespace details {
	    template <class T, class = void>
	    struct exists_ : std::false_type {};

	    template <class T>
	    struct exists_<T, void_t<T>> : std::true_type {};
	}  // namespace details

	/**
	 * Determines whether or not the given type is well-formed
	 * @tparam T being the type to check
	 * @return TRUE if well formed, FALSE otherwise
	 */
	template <class T>
	inline constexpr bool exists() {
	    return details::exists_<T>::value;
	}
    }  // namespace meta
}  // namespace scl