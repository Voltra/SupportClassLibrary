#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			/*template <class From, class To>
			struct is_convertible{
				static constexpr bool v = std::is_convertible<From, To>::value;
			};*/

			/**
			 * An handy function for the result of std::is_convertible
			 */
			template <class From, class To>
			inline constexpr bool is_convertible(){
				return std::is_convertible<From, To>::value;
			}
		}
	}
}