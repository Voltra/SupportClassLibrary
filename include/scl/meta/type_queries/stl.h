#pragma once
#include <type_traits>

#include "../../macros.h"

namespace scl {
    namespace meta {
	/**
	 * An handy function for the result of std::is_base_of
	 * @tparam Base being the base class we want to know if the derived class extends from
	 * @tparam Derived being the queried type
	 */
	template <class Base, class Derived>
	inline constexpr bool is_base_of() {
	    return std::is_base_of<Base, Derived>::value;
	}

	/**
	 * An handy function for the result of std::is_convertible
	 * @tparam From being the type we want to convert from
	 * @tparam To being the type we want to convert to
	 */
	template <class From, class To>
	inline constexpr bool is_convertible() {
	    return std::is_convertible<From, To>::value;
	}

	/**
	 * An handy function for the result of std::is_same
	 */
	template <class Lhs, class Rhs>
	inline constexpr bool is_same() {
	    return std::is_same<Lhs, Rhs>::value;
	}

	template <class T>
	inline constexpr bool is_void() {
	    return std::is_void<T>::value;
	}

	template <class T>
	inline constexpr bool is_null_pointer() {
	    return std::is_null_pointer<T>::value;
	}

	template <class T>
	inline constexpr bool is_integral() {
	    return std::is_integral<T>::value;
	}

	template <class T>
	inline constexpr bool is_floating_point() {
	    return std::is_floating_point<T>::value;
	}

	template <class T>
	inline constexpr bool is_array() {
	    return std::is_array<T>::value;
	}

	template <class T>
	inline constexpr bool is_enum() {
	    return std::is_enum<T>::value;
	}

	template <class T>
	inline constexpr bool is_union() {
	    return std::is_union<T>::value;
	}

	template <class T>
	inline constexpr bool is_class() {
	    return std::is_class<T>::value;
	}

	template <class T>
	inline constexpr bool is_function() {
	    return std::is_function<T>::value;
	}

	template <class T>
	inline constexpr bool is_pointer() {
	    return std::is_pointer<T>::value;
	}

	template <class T>
	inline constexpr bool is_lvalue_reference() {
	    return std::is_lvalue_reference<T>::value;
	}

	template <class T>
	inline constexpr bool is_rvalue_reference() {
	    return std::is_rvalue_reference<T>::value;
	}

	template <class T>
	inline constexpr bool is_member_object_pointer() {
	    return std::is_member_object_pointer<T>::value;
	}

	template <class T>
	inline constexpr bool is_member_function_pointer() {
	    return std::is_member_function_pointer<T>::value;
	}

	template <class T>
	inline constexpr bool is_fundamental() {
	    return std::is_fundamental<T>::value;
	}

	template <class T>
	inline constexpr bool is_arithmetic() {
	    return std::is_arithmetic<T>::value;
	}

	template <class T>
	inline constexpr bool is_scalar() {
	    return std::is_scalar<T>::value;
	}

	template <class T>
	inline constexpr bool is_object() {
	    return std::is_object<T>::value;
	}

	template <class T>
	inline constexpr bool is_compound() {
	    return std::is_compound<T>::value;
	}

	template <class T>
	inline constexpr bool is_reference() {
	    return std::is_reference<T>::value;
	}

	template <class T>
	inline constexpr bool is_member_pointer() {
	    return std::is_member_pointer<T>::value;
	}

	template <class T>
	inline constexpr bool is_const() {
	    return std::is_const<T>::value;
	}

	template <class T>
	inline constexpr bool is_volatile() {
	    return std::is_volatile<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivial() {
	    return std::is_trivial<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_copyable() {
	    return std::is_trivially_copyable<T>::value;
	}

	template <class T>
	inline constexpr bool is_standard_layout() {
	    return std::is_standard_layout<T>::value;
	}

	template <class T>
	inline constexpr bool is_empty() {
	    return std::is_empty<T>::value;
	}

	template <class T>
	inline constexpr bool is_polymorphic() {
	    return std::is_polymorphic<T>::value;
	}

	template <class T>
	inline constexpr bool is_abstract() {
	    return std::is_abstract<T>::value;
	}

	template <class T>
	inline constexpr bool is_signed() {
	    return std::is_signed<T>::value;
	}

	template <class T>
	inline constexpr bool is_unsigned() {
	    return std::is_unsigned<T>::value;
	}

	template <class T, class... Args>
	inline constexpr bool is_constructible() {
	    return std::is_constructible<T, Args...>::value;
	}

	template <class T, class... Args>
	inline constexpr bool is_trivially_constructible() {
	    return std::is_trivially_constructible<T, Args...>::value;
	}

	template <class T, class... Args>
	inline constexpr bool is_nothrow_constructible() {
	    return std::is_nothrow_constructible<T, Args...>::value;
	}

	template <class T>
	inline constexpr bool is_default_constructible() {
	    return std::is_default_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_default_constructible() {
	    return std::is_trivially_default_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_default_constructible() {
	    return std::is_nothrow_default_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_copy_constructible() {
	    return std::is_copy_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_copy_constructible() {
	    return std::is_trivially_copy_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_copy_constructible() {
	    return std::is_nothrow_copy_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_move_constructible() {
	    return std::is_move_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_move_constructible() {
	    return std::is_trivially_move_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_move_constructible() {
	    return std::is_nothrow_move_constructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_copy_assignable() {
	    return std::is_copy_assignable<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_copy_assignable() {
	    return std::is_trivially_copy_assignable<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_copy_assignable() {
	    return std::is_nothrow_copy_assignable<T>::value;
	}

	template <class T>
	inline constexpr bool is_move_assignable() {
	    return std::is_move_assignable<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_move_assignable() {
	    return std::is_trivially_move_assignable<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_move_assignable() {
	    return std::is_nothrow_move_assignable<T>::value;
	}

	template <class T>
	inline constexpr bool is_destructible() {
	    return std::is_destructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_trivially_destructible() {
	    return std::is_trivially_destructible<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_destructible() {
	    return std::is_nothrow_destructible<T>::value;
	}

	template <class T>
	inline constexpr bool has_virtual_destructor() {
	    return std::has_virtual_destructor<T>::value;
	}

	template <class To, class From>
	inline constexpr bool is_assignable() {
	    return std::is_assignable<To, From>::value;
	}

	template <class To, class From>
	inline constexpr bool is_trivially_assignable() {
	    return std::is_trivially_assignable<To, From>::value;
	}

	template <class To, class From>
	inline constexpr bool is_nothrow_assignable() {
	    return std::is_nothrow_assignable<To, From>::value;
	}

#ifdef SCL_CPP14
	template <class T>
	inline constexpr bool is_final() {
	    return std::is_final<T>::value;
	}
#endif

#ifdef SCL_CPP17
	template <class T>
	inline constexpr bool is_aggregate() {
	    return std::is_aggregate<T>::value;
	}

	template <class T>
	inline constexpr bool has_unique_object_representations() {
	    return std::has_unique_object_representations<T>::value;
	}

	template <class T>
	inline constexpr bool is_swappable() {
	    return std::is_swappable<T>::value;
	}

	template <class T>
	inline constexpr bool is_nothrow_swappable() {
	    return std::is_nothrow_swappable<T>::value;
	}

	template <class Lhs, class Rhs>
	inline constexpr bool is_swappable_with() {
	    return std::is_swappable_with<Lhs, Rhs>::value;
	}

	template <class Lhs, class Rhs>
	inline constexpr bool is_swappable_with() {
	    return std::is_nothrow_swappable_with<Lhs, Rhs>::value;
	}

	template <class Fn, class... Args>
	inline constexpr bool is_invocable() {
	    return std::is_invocable<Fn, Args...>::value;
	}

	template <class R, class Fn, class... Args>
	inline constexpr bool is_invocable_r() {
	    return std::is_invocable_r<R, Fn, Args...>::value;
	}

	template <class Fn, class... Args>
	inline constexpr bool is_nothrow_invocable() {
	    return std::is_nothrow_invocable<Fn, Args...>::value;
	}

	template <class R, class Fn, class... Args>
	inline constexpr bool is_nothrow_invocable_r() {
	    return std::is_nothrow_invocable<R, Fn, Args...>::value;
	}
#endif
    }  // namespace meta
}  // namespace scl