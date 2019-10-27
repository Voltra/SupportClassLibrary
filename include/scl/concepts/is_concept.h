#pragma once

#include <type_traits>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/is_convertible.h>
#include <scl/tools/meta/type_check.h>
#include <scl/macros.h>

namespace scl{
	namespace concepts{
		namespace details{
			template <class T, class = void>
			struct __is_concept : std::false_type{};

			template <class T>
			struct __is_concept<T,  META::enable_if_t<
				META::is_convertible<T, bool>()
				&& META::is_default_constructible<T>()
			>> : std::true_type{};
		}

		/**
		 * Determine whether or not a given type is a concept-like type
		 * @tparam T being the type to test
		 * @return TRUE if it is a concept-like type, FALSE otherwise
		 */
		template <class T>
		constexpr bool is_concept(){
			return details::__is_concept<T>::value;
		}

		/*
		 A concept type can be defined as follows:
		 template <class T>
		 struct Integral{
		 	constexpr operator bool() const{
		 		static_assert(meta::is_integral<T>(), "Fails to meet the requirements of Integral concept");
		 		return true;
		 	}
		 }

		 They are defined in such a way that you can easily compose them via the require function:
		 template <class T>
		 struct Copyable{
		 	constexpr operator bool() const{
		 		require(CopyConstructible<T>{});
		 		require(CopyAssignable<T>{});
		 		return true;
		 	}
		 }
		 */
	}
}
#undef SCL_META