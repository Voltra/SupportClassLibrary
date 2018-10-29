#pragma once

#include <scl/tools/meta/is_base_of.h>

namespace scl{
	namespace concepts{
		template <class Derived, class Base>
		struct DerivedFrom{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_base_of<Base, Derived>(), "Derived is not derived from Base");
				return true;
			}
		};
	}
}