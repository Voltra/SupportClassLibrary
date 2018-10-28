#pragma once

#include <type_traits>
#include <scl/tools/meta/is_same.h>

namespace scl{
	namespace tools{
		namespace meta{
			template <class Ret, class... Args>
			using fn_type = Ret(*)(Args...);

			template <class Class, class Ret, class... Args>
			using memfn_type = Ret(Class::*)(Args...);
		}
	}
}