#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * An handy function for the result of std::is_base_of
			 */
			template <class Base, class Derived>
			inline constexpr bool is_base_of(){
				return std::is_base_of<Base, Derived>::value;
			}
		}
	}
}