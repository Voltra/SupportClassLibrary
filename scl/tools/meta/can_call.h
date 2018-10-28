#pragma once

#include <type_traits>
#include <scl/tools/meta/is_same.h>

namespace scl{
	namespace tools{
		namespace meta{
			template <class Func, class... Args>
			inline constexpr auto __can_call_impl(Func f, bool)
			-> decltype(f(std::declval<Args>()...), std::true_type{}){
				return std::true_type{};
			}

			template <class Func, class...>
			inline constexpr std::false_type __can_call_impl(Func, ...){
				return std::false_type{};
			}

			template <class Func, class... Args>
			inline constexpr bool can_call(Func f){
				return is_same<
				    decltype(__can_call_impl(f, true)),
				    std::true_type
				>();
			}
		}
	}
}