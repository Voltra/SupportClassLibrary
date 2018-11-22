#pragma once

#include <type_traits>
#include <scl/tools/meta/is_same.h>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * An handy type alias to describe function pointer types
			 * @tparam Ret being the return type of the function pointer
			 * @tparam Args being the argument's types
			 */
			template <class Ret, class... Args>
			using fn_type = Ret(*)(Args...);

			/**
			 * An handy type alias to describe member function pointer types
			 * @tparam Class being the class this memfn is from
			 * @tparam Ret being the return type of the member function pointer
			 * @tparam Args being the argument's types
			 */
			template <class Class, class Ret, class... Args>
			using memfn_type = Ret(Class::*)(Args...);
		}
	}
}