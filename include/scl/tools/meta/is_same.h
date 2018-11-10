#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * An handy function for the result of std::is_same
			 */
			template <class Lhs, class Rhs>
			inline constexpr bool is_same(){
				return std::is_same<Lhs, Rhs>::value;
			}
		}
	}
}