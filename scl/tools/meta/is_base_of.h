#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			template <class Base, class Derived>
			inline constexpr bool is_base_of(){
				return std::is_base_of<Lhs, Rhs>::value;
			}
		}
	}
}