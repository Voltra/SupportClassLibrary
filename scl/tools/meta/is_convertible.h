#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			/*template <class From, class To>
			struct is_convertible{
				static constexpr bool v = std::is_convertible<From, To>::value;
			};*/

			template <class From, class To>
			inline constexpr bool is_convertible(){
				return std::is_convertible<From, To>::value;
			}
		}
	}
}