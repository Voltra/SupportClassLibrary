#pragma once

#include "../tools/meta/is_same.h"

namespace scl{
	namespace concepts{
		/**
		 * Same concept, two types Lhs and Rhs are considered the same if meta::is_same<Lhs, Rhs>() is true
		 * @tparam Lhs being the left hand side type
		 * @tparam Rhs being th right hand side type
		 */
		template <class Lhs, class Rhs>
		struct Same{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_same<Lhs, Rhs>(), "Same<Lhs, Rhs>: Lhs and Rhs are different types.");
				return true;
			}
		};
	}
}