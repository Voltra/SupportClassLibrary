#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/CopyAssignable.h>
#include <scl/concepts/CopyConstructible.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct Copyable{
			constexpr operator bool() const{
				require(CopyConstructible<T>{});
				require(CopyAssignable<T>{});
				return true;
			}
		};
	}
}