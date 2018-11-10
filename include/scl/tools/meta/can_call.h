#pragma once

#include <type_traits>
#include "is_same.h"

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

			/**
			 * Determines whether or not the given function can be called with the given argument types
			 * @tparam Func being the type of the function (auto deduction)
			 * @tparam Args being the arguments' types
			 * @param f being the desired function
			 * @return TRUE if it can be called using these arguments, FALSE otherwise
			 */
			template <class Func, class... Args>
			inline constexpr bool can_call(Func f){
				return is_same<
				    decltype(__can_call_impl<Func, Args...>(f, true)),
				    std::true_type
				>();
			}
		}
	}
}