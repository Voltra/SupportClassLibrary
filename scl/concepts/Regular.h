#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/SemiRegular.h>
#include <scl/concepts/EqualityComparable.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct Regular{
			constexpr operator bool() const{
				require(SemiRegular<T>{});
				require(EqualityComparable<T>{});
				return true;
			}
		};
	}
}