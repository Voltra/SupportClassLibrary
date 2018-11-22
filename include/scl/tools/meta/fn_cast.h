#pragma once

#include <type_traits>
#include <scl/tools/meta/is_same.h>
#include <scl/tools/meta/fn_type.h>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * Cast a function from its type to determined type
			 * @tparam From being the actual type of the function
			 * @tparam Ret being the new return type
			 * @tparam Args being the new arguments' types
			 * @param f being the function to cast
			 * @return the function pointer statically casted
			 */
			template <class From, class Ret, class... Args>
			inline constexpr fn_type<Ret, Args...> fn_cast(From f){
				return static_cast<fn_type<Ret, Args...>>(f);
			}

			/**
			 * Cast a member function from its type to determined type
			 * @tparam From being the actual type of the function
			 * @tparam Class being the new class it is from
			 * @tparam Ret being the new return type
			 * @tparam Args being the new arguments' types
			 * @param f being the function to cast
			 * @return the member function pointer statically casted
			 */
			template <class From, class Class, class Ret, class... Args>
			inline constexpr memfn_type<Class, Ret, Args...> fn_cast(From f){
				return static_cast<memfn_type<Class, Ret, Args...>>(f);
			}
		}
	}
}