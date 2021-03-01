#pragma once
#include <functional>
#include <type_traits>
//#include <scl/tools/meta/is_same.h>

namespace scl{
	namespace tools{
		namespace meta{
			namespace fn_types{
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
                template <class Ret, class Class, class... Args>
                using mem_fn_type = Ret(Class::*)(Args...);

                template <class Ret, class Class, class... Args>
                using const_mem_fn_type = Ret(Class::*)(Args...) const;

                template <class Ret, class Class>
                using mem_ptr_type = Ret Class::*;

                template <class Ret, class... Args>
                using std_type = std::function<Ret(Args...)>;

                template <class Ret, class... Args>
                using sig_type = Ret(Args...);
			}
		}
	}
}