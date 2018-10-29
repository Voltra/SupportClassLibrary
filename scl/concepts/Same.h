#pragma once

#include <scl/tools/meta/is_same.h>

namespace scl{
	namespace concepts{
		template <class Lhs, class Rhs>
		struct Same{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_same<Lhs, Rhs>(), "Lhs and Rhs are different types.");
				return true;
			}
		};
	}
}