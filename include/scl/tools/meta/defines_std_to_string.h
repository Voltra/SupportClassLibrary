#pragma once

#include <type_traits>
#include <string>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/type_mod.h>

namespace scl{
	namespace tools{
		namespace meta{
			template <class T, class = void>
			struct __to_string_defined : std::false_type{};

			template <class T>
			struct __to_string_defined<T, /*enable_if_t*/void_t<
				/*exists<*/decltype(std::to_string(std::declval<const T&>()))/*>()*/
			>> : std::true_type{};

			/**
			 * Determines whether or not std::to_string(const T&) is defined
			 * @tparam T being the type to test upon
			 * @return TRUE if defined, FALSE otherwise
			 */
			template <class T>
			inline constexpr bool defines_std_to_string(){
				return __to_string_defined<decay_t<T>>::value;
			}
		}
	}
}