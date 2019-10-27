#pragma once

#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/utils/toString.h>

namespace scl{
	namespace tools{
		namespace meta{
			namespace details{
				template <class T, class = void>
				struct __toStringDefined : std::false_type{
				};

				template <class T>
				struct __toStringDefined<T, void_t<scl::utils::ToString<T>>> : std::true_type{
				};
			}

			/**
			 * Determines whether or not ToString has been specialized for the given type
			 * @tparam T being the type to test upon
			 * @return TRUE if ToString<T> is defined, FALSE otherwise
			 */
			template <class T>
			inline constexpr bool defines_scl_to_string(){
				return details::__toStringDefined<T>::value;
			}
		}
	}
}