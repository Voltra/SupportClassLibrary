#pragma once

#ifdef SCL_USE_IOSTREAM_EXTEND
#include <string>
#include <utility>
#include <scl/macros.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/defines_std_to_string.h>
#include <scl/tools/meta/defines_scl_to_string.h>
#include <scl/utils/toString.h>

namespace std{
	/**
	 * Define std::to_string if not defined but there's a scl::utils::ToString for it
	 * @tparam T being the value type
	 * @param obj being the object to convert to a string
	 * @return the string representation of obj according to scl::utils::toString(obj)
	 */
	template <class T, class = META::void_t<META::enable_if_t<
		!META::defines_std_to_string<T>()
		&& META::defines_scl_to_string<T>()
	>>>
	std::string to_string(T&& obj){
		return scl::asString(std::forward<T>(obj));
	}
}

namespace scl{
	namespace utils{
		/**
		 * Specializes scl::utils::ToString if not defined but there's a std::to_string for it
		 * @tparam T being the value type
		 */
		template <class T>
		struct ToString<T, META::enable_if_t<
			!META::defines_scl_to_string<T>()
			&& META::defines_std_to_string<T>()
		>>{
			std::string operator()(T& obj) const{
				return std::to_string(obj);
			}
		};
	}
}

//The following will add the definitions
#include "toString.h"
#endif

//Note: to enable these operators, you must be in/use the namespace scl::tools::iostream