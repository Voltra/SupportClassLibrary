#pragma once

#include <type_traits>

#define SCL_TYPEQUERY_V(ret, fn) \
template <class T>\
inline constexpr ret fn(){\
	return std::fn<T>::value;\
}

#define SCL_TYPEQUERY(alias) \
template <class... T>\
using alias##_t = typename std::alias<T...>::type;

namespace scl{
	namespace tools{
		namespace meta{
			SCL_TYPEQUERY_V(std::size_t, alignment_of)
			SCL_TYPEQUERY_V(std::size_t, rank)

			template <class T, unsigned N = 0>
			inline constexpr std::size_t extent(){
				return std::extent<T, N>::value;
			}

			template <std::size_t Len, std::size_t Align = /*Len **/ sizeof(char)>
			using aligned_storage_t = typename std::aligned_storage<Len, Align>::type;

			template <std::size_t Len, class... Types>
			using aligned_union_t = typename std::aligned_union<Len, Types...>::type;

			SCL_TYPEQUERY(common_type)

			template <class T>
			using underlying_type_t = typename std::underlying_type<T>::type;

			template <size_t I, class Tuple>
			using tuple_element_t = typename std::tuple_element<I, Tuple>::type;

#ifdef SCL_CPP17
			template <class F, class... Args>
			using invoke_result_t = typename std::invoke_result<F, Args...>::type;
#else
			template <class Signature>
			using result_of_t = typename std::result_of<Signature>::type;
#endif
		}
	}
}

#undef SCL_TYPEQUERY_V
#undef SCL_TYPEQUERY