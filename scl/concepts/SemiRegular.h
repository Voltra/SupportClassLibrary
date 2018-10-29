#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Copyable.h>
#include <scl/concepts/DefaultConstructible.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct SemiRegular{
			constexpr operator bool() const{
				require(Copyable<T>{});
				require(DefaultConstructible<T>{});
				return true;
			}
		};
	}
}