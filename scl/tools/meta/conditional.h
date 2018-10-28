#pragma once

#include <type_traits>

namespace scl{
	namespace tools{
		namespace meta{
			template <bool B, class T, class F>
			using conditional_t = typename std::conditional<B, T, F>::type;
		}
	}
}