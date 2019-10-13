#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <scl/macros.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/defines_scl_to_string.h>
#include <scl/tools/meta/defines_ostream_operator.h>
#include <scl/tools/meta/is_convertible.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/utils/toString.h>


/**
 * Automatically define the ostream operator<< when scl::utils::ToString is specialized
 * @tparam T being the value type to output
 * @param os being the output stream
 * @param obj the object to output on the output stream
 * @return a reference to the output stream
 */
template <
	class T,
	class T_ = META::decay_t<T>,
	class = META::void_t<META::enable_if_t<
		!(
			META::is_same<T_, char>()
			|| META::is_same<T_, const char*>()
			|| META::is_same<T_, std::string>()
			|| META::defines_ostream_operator<T_>()
		) && scl::tools::meta::defines_scl_to_string<T>()
	>>
>
std::ostream& operator<<(std::ostream& os, T&& obj){
	return os << scl::utils::toString<T_>(std::forward<T>(obj));
}
