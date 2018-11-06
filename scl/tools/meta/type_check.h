#pragma once

#include <type_traits>
#ifndef SCL_CPP17
	#include <scl/tools/meta/can_call.h>
	#include <scl/tools/meta/exists.h>
#endif

#define SCL_TYPECHECK(fn) \
template <class T>\
inline constexpr bool fn(){\
	return std::fn<T>::value;\
}

#define SCL_TYPECHECK_COMPOUND(fn) \
template <class T, class U>\
inline constexpr bool fn(){\
	return std::fn<T, U>::value;\
}

#define SCL_TYPECHECK_WITH_VARGS(fn)\
template <class T, class... Args>\
inline constexpr bool fn(){\
	return std::fn<T, Args...>::value;\
}

#define SCL_TYPECHECK_FN(fn)\
template <class F, class... Args>\
inline constexpr bool fn(){\
	return std::fn<F, Args...>::value;\
}

#define SCL_TYPECHECK_FN_R(fn)\
template <class R, class F, class... Args>\
inline constexpr bool fn(){\
	return std::fn<R, F, Args...>::value;\
}

namespace scl{
	namespace tools{
		namespace meta{
			SCL_TYPECHECK(is_void)
			SCL_TYPECHECK(is_null_pointer)
			SCL_TYPECHECK(is_integral)
			SCL_TYPECHECK(is_floating_point)
			SCL_TYPECHECK(is_array)
			SCL_TYPECHECK(is_enum)
			SCL_TYPECHECK(is_union)
			SCL_TYPECHECK(is_class)
			SCL_TYPECHECK(is_function)
			SCL_TYPECHECK(is_pointer)
			SCL_TYPECHECK(is_lvalue_reference)
			SCL_TYPECHECK(is_rvalue_reference)
			SCL_TYPECHECK(is_member_object_pointer)
			SCL_TYPECHECK(is_member_function_pointer)

			SCL_TYPECHECK(is_fundamental)
			SCL_TYPECHECK(is_arithmetic)
			SCL_TYPECHECK(is_scalar)
			SCL_TYPECHECK(is_object)
			SCL_TYPECHECK(is_compound)
			SCL_TYPECHECK(is_reference)
			SCL_TYPECHECK(is_member_pointer)

			SCL_TYPECHECK(is_const)
			SCL_TYPECHECK(is_volatile)
			SCL_TYPECHECK(is_trivial)
			SCL_TYPECHECK(is_trivially_copyable)
			SCL_TYPECHECK(is_standard_layout)
			SCL_TYPECHECK(is_empty)
			SCL_TYPECHECK(is_polymorphic)
			SCL_TYPECHECK(is_abstract)
			SCL_TYPECHECK(is_signed)
			SCL_TYPECHECK(is_unsigned)
#ifdef SCL_ENABLE_DEPRECATED_TYPECHECKS
			SCL_TYPECHECK(is_pod)
			SCL_TYPECHECK(is_literal_type)
#endif

#ifdef SCL_CPP14
			SCL_TYPECHECK(is_final)
#endif

#ifdef SCL_CPP17
			SCL_TYPECHECK(is_aggregate)
			SCL_TYPECHECK(has_unique_object_representations)
#endif

			SCL_TYPECHECK_WITH_VARGS(is_constructible)
			SCL_TYPECHECK_WITH_VARGS(is_trivially_constructible)
			SCL_TYPECHECK_WITH_VARGS(is_nothrow_constructible)

			SCL_TYPECHECK(is_default_constructible)
			SCL_TYPECHECK(is_trivially_default_constructible)
			SCL_TYPECHECK(is_nothrow_default_constructible)

			SCL_TYPECHECK(is_copy_constructible)
			SCL_TYPECHECK(is_trivially_copy_constructible)
			SCL_TYPECHECK(is_nothrow_copy_constructible)

			SCL_TYPECHECK(is_move_constructible)
			SCL_TYPECHECK(is_trivially_move_constructible)
			SCL_TYPECHECK(is_nothrow_move_constructible)

			SCL_TYPECHECK(is_copy_assignable)
			SCL_TYPECHECK(is_trivially_copy_assignable)
			SCL_TYPECHECK(is_nothrow_copy_assignable)

			SCL_TYPECHECK(is_move_assignable)
			SCL_TYPECHECK(is_trivially_move_assignable)
			SCL_TYPECHECK(is_nothrow_move_assignable)

			SCL_TYPECHECK(is_destructible)
			SCL_TYPECHECK(is_trivially_destructible)
			SCL_TYPECHECK(is_nothrow_destructible)
			SCL_TYPECHECK(has_virtual_destructor)

#ifdef SCL_CPP17
			SCL_TYPECHECK(is_swappable)
			SCL_TYPECHECK(is_nothrow_swappable)
			SCL_TYPECHECK_COMPOUND(is_swappable_with)
			SCL_TYPECHECK_COMPOUND(is_nothrow_swappable_with)

			SCL_TYPECHECK_FN(is_invocable)
			SCL_TYPECHECK_FN(is_nothrow_invocable)
			SCL_TYPECHECK_FN(is_invocable_r)
			SCL_TYPECHECK_FN_R(is_nothrow_invocable_r)
#else
			template <class F, class... Args>
			inline constexpr bool is_invocable(){
//				return can_call<F, Args...>(std::declval<F>());
				return exists<decltype(std::declval<F>()(std::declval<Args>()...))>();
			}
#endif
		}
	}
}

#ifndef SCL_CPP17
	#include <scl/tools/meta/swap.h>
#endif

#undef SCL_TYPECHECK
#undef SCL_TYPECHECK_COMPOUND
#undef SCL_TYPECHECK_WITH_VARGS
#undef SCL_TYPECHECK_FN
#undef SCL_TYPECHECK_FN_R