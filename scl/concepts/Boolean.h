#pragma once

#include <scl/tools/meta/type_check.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/concepts/require.h>
#include <scl/concepts/Movable.h>
#include <scl/concepts/ConvertibleTo.h>
#include <scl/concepts/EqualityComparable.h>
#include <scl/concepts/EqualityComparableWith.h>
#include <scl/concepts/InequalityComparable.h>
#include <scl/concepts/InequalityComparableWith.h>

namespace scl{
	namespace concepts{
		/**
		 * Boolean concept, a type is boolean if it defines all boolean operations
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Boolean{
			constexpr operator bool() const{
				using namespace scl::tools;
				require(Movable<meta::remove_cv_t<T>>{});

				using noref = meta::remove_reference_t<T>;
				require(ConvertibleTo<noref, bool>{});

				require(ConvertibleTo<decltype(!std::declval<T>()), bool>{});
				require(ConvertibleTo<decltype(std::declval<T>() && true), bool>{});
				require(ConvertibleTo<decltype(std::declval<T>() || true), bool>{});


				require(ConvertibleTo<decltype(std::declval<T>() && std::declval<T>()), bool>{});
				require(ConvertibleTo<decltype(std::declval<T>() || std::declval<T>()), bool>{});


				require(ConvertibleTo<decltype(true && std::declval<T>()), bool>{});
				require(ConvertibleTo<decltype(true || std::declval<T>()), bool>{});

				require(EqualityComparable<T>{});
				require(EqualityComparableWith<T, bool>{});
				require(EqualityComparableWith<bool, T>{});

				require(InequalityComparable<T>{});
				require(InequalityComparableWith<T, bool>{});
				require(InequalityComparableWith<bool, T>{});

				static_assert(meta::is_nothrow_destructible<T>(), "T is not destructible");
				return true;
			}
		};
	}
}