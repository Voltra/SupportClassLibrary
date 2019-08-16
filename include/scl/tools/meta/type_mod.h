#pragma once

#include <type_traits>

#define SCL_TYPEMOD(alias) \
template <class T>\
using alias##_t = typename std::alias<T>::type;

namespace scl{
	namespace tools{
		namespace meta{
			SCL_TYPEMOD(remove_cv)
			SCL_TYPEMOD(remove_const)
			SCL_TYPEMOD(remove_volatile)

			SCL_TYPEMOD(add_cv)
			SCL_TYPEMOD(add_const)
			SCL_TYPEMOD(add_volatile)

			SCL_TYPEMOD(remove_reference)
			SCL_TYPEMOD(add_lvalue_reference)
			SCL_TYPEMOD(add_rvalue_reference)

			SCL_TYPEMOD(remove_pointer)
			SCL_TYPEMOD(add_pointer)

			SCL_TYPEMOD(make_signed)
			SCL_TYPEMOD(make_unsigned)

			SCL_TYPEMOD(remove_extent)
			SCL_TYPEMOD(remove_all_extents)

			SCL_TYPEMOD(decay)

			template <class T>
			using remove_cv_ref_t = remove_cv_t<remove_reference_t<T>>;

			/**
			 * Type construct that exposes a single type that is its type argument
			 * @tparam T being the type argument to expose
			 */
			template <class T>
			struct NoOp{ using type = T; };

			template <class T>
			using no_op_t = typename NoOp<T>::type;

#ifdef SCL_ENABLE_DEPRECATED_TYPEMODS
			SCL_TYPEMOD(result_of)
#endif
		}
	}
}

#undef SCL_TYPEMOD