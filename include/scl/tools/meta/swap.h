#pragma once

#ifndef SCL_CPP17
	#include <type_traits>
	#include <utility>
	#include <scl/tools/meta/enable_if.h>
	#include <scl/tools/meta/exists.h>
	#include <scl/tools/meta/is_same.h>
	#include <scl/tools/meta/type_mod.h>

	#define SCL_t std::declval<T&>()
	#define SCL_u std::declval<U&>()

	namespace scl{
		namespace tools{
			namespace meta{
				namespace details{
					template <class T, class U, class = void>
					struct __is_swappable_with : std::false_type{};

					template <class T, class U>
					struct __is_swappable_with<T, U, void_t<enable_if_t<
													 exists<decltype(std::swap(SCL_t, SCL_u))>()
													 && exists<decltype(std::swap(SCL_u, SCL_t))>()
					>>> : std::true_type{};

					template <class T, class U>
					constexpr bool is_swappable_with(){
						return __is_swappable_with<T, U>::value;
					}

					template <class T, class = void>
					struct __is_swappable : std::false_type{};

					template <class T>
					struct __is_swappable<T, enable_if_t<
											 !is_same<T, add_lvalue_reference_t<T>>()
					&& is_swappable_with<T&, T&>()
					>> : std::true_type{};
				}

				template <class T>
				constexpr bool is_swappable(){
					return details::__is_swappable<T>::value;
				}
			}
		}
	}

	#undef SCL_t
	#undef SCL_u
#endif
