#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * An handy type alias for the result of std::conditional
			 */
			template <bool B, class T, class F>
			using conditional_t = typename std::conditional<B, T, F>::type;
		}
	}
}