#pragma once

#include <scl/tools/meta/is_base_of.h>

namespace scl{
	namespace concepts{
		/**
		 * DerivedFrom concept, a type Derived is derived from another type Base if Derived is a derives from Base
		 * @tparam Derived being the supposed derived type
		 * @tparam Base being the supposed base type
		 */
		template <class Derived, class Base>
		struct DerivedFrom{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_base_of<Base, Derived>(), "DerivedFrom<Derived, Base>: Derived is not derived from Base");
				return true;
			}
		};
	}
}