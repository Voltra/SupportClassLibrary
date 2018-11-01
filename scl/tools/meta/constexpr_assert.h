#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			template <bool b>
			inline constexpr bool constexpr_assert(){
				static_assert(b, "");
				return true;
			}
		}
	}
}