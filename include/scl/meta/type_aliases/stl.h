#pragma once
#include <type_traits>
#include <utility>

#include "../../macros.h"

namespace scl {
    namespace meta {
	/**
	 * An handy type alias for the result of std::conditional
	 */
	template <bool Boolean, class TrueType, class FalseType>
	using conditional_t = typename std::conditional<Boolean, TrueType, FalseType>::type;

	/**
	 * An handy type alias for the result of std::enable_if
	 */
	template <bool B, class T = void>
	using enable_if_t = typename std::enable_if<B, T>::type;

	template <class T>
	using remove_cv_t = typename std::remove_cv<T>::type;

	template <class T>
	using remove_const_t = typename std::remove_const<T>::type;

	template <class T>
	using remove_volatile_t = typename std::remove_volatile<T>::type;

	template <class T>
	using add_cv_t = typename std::add_cv<T>::type;

	template <class T>
	using add_const_t = typename std::add_const<T>::type;

	template <class T>
	using add_volatile_t = typename std::add_volatile<T>::type;

	template <class T>
	using remove_reference_t = typename std::remove_reference<T>::type;

	template <class T>
	using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

	template <class T>
	using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

	template <class T>
	using add_pointer_t = typename std::add_pointer<T>::type;

	template <class T>
	using remove_pointer_t = typename std::remove_pointer<T>::type;

	template <class T>
	using make_signed_t = typename std::make_signed<T>::type;

	template <class T>
	using make_unsigned_t = typename std::make_unsigned<T>::type;

	template <class T>
	using remove_extent_t = typename std::remove_extent<T>::type;

	template <class T>
	using remove_all_extents_t = typename std::remove_all_extents<T>::type;

	template <class T>
	using decay_t = typename std::decay<T>::type;

	template <class T>
	using remove_cv_ref_t = remove_cv_t<remove_reference_t<T>>;

	namespace details {
	    /**
	     * Type construct that exposes a single type that is its type argument
	     * @tparam T being the type argument to expose
	     */
	    template <class T>
	    struct NoOp {
		    using type = T;
	    };
	}  // namespace details

	template <class T>
	using no_op_t = typename details::NoOp<T>::type;

	template <std::size_t Len, std::size_t Align = sizeof(char)>
	using aligned_storage_t = typename std::aligned_storage<Len, Align>::type;

	template <std::size_t Len, class... Types>
	using aligned_union_t = typename std::aligned_union<Len, Types...>::type;

	template <class... T>
	using common_type_t = typename std::common_type<T...>::type;

	template <class T>
	using underlying_type_t = typename std::underlying_type<T>::type;

	template <std::size_t I, class Tuple>
	using tuple_element_t = typename std::tuple_element<I, Tuple>::type;
    }  // namespace meta
}  // namespace scl