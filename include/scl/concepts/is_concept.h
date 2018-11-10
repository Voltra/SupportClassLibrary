#pragma once

#include <type_traits>
#include "../tools/meta/enable_if.h"
#include "../tools/meta/is_convertible.h"

#define SCL_META scl::tools::meta
namespace scl{
	namespace concepts{
		template <class T, class = void>
		struct __is_concept : std::false_type{};

		template <class T>
		struct __is_concept<T,  SCL_META::enable_if_t<
			SCL_META::is_convertible<T, bool>()
		>>: std::true_type{};

		/**
		 * Determine whether or not a given type is a concept-like type
		 * @tparam T being the type to test
		 * @return TRUE if it is a concept-like type, FALSE otherwise
		 */
		template <class T>
		constexpr bool is_concept(){
			return __is_concept<T>::value;
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