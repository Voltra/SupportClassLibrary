#pragma once
#include <type_traits>

namespace scl {
    namespace meta {
	namespace details {
	    template <template <class, class...> class, class...>
	    struct is_instance : std::false_type {};

	    template <template <class, class...> class U, class... T>
	    struct is_instance<U, U<T...>> : std::true_type {};
	}  // namespace details

	/**
	 * Determines whether a template type is an "instance" of a templated type
	 * @tparam U being the templated type
	 * @tparam T being the types composing the templated type (if the type to check is
	 * "instance")
	 * @return TRUE if it is "instance", FALSE otherwise
	 */
	template <template <class, class...> class U, class... T>
	inline constexpr bool is_instance() {
	    return details::is_instance<U, T...>::value;
	}
    }  // namespace meta
}  // namespace scl