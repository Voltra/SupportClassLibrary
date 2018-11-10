#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * An handy type alias for the result of std::enable_if
			 */
			template <bool B, class T = void>
			using enable_if_t = typename std::enable_if<B, T>::type;
		}
	}
}