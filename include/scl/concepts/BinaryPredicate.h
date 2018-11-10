#pragma once

#include "../macros.h"
#include "Predicate.h"
#include "Iterator.h"

namespace scl{
	namespace concepts{
		/**
		 * BinaryPredicate concept, a type F is a binary predicate over T and U
		 * if it is an n-ary predicate over those types
		 * @tparam F being the type to check against
		 * @tparam T being the first argument's type
		 * @tparam U being the second/last argument's type
		 */
		template <class F, class T, class U>
		struct BinaryPredicate{
			constexpr operator bool() const{
				return Predicate<F, T, U>{};
			}
		};

		/*template <class F, class It, class It_>
		struct BinaryPredicate{
			constexpr operator bool() const{
				return Iterator<It>{}
					   && Iterator<It_>{}
					   && Predicate<F, typename It::value_type, typename It_::valuetype>{};
//				&& CopyConstructible<F>{};
			}
		};*/
	}
}