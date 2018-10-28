#pragma once

#include <type_traits>
#include <scl/tools/meta/is_same.h>
#include <scl/tools/meta/fn_type.h>

namespace scl{
	namespace tools{
		namespace meta{
			template <class From, class Ret, class... Args>
			inline constexpr fn_type<Ret, Args...> fn_cast(From f){
				return static_cast<fn_type<Ret, Args...>>(f);
			}

			template <class From, class Class, class Ret, class... Args>
			inline constexpr memfn_type<Class, Ret, Args...> fn_cast(From f){
				return static_cast<memfn_type<Class, Ret, Args...>>(f);
			}
		}
	}
}