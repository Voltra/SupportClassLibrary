#pragma once

#include "void_t.h"
#include "type_mod.h"
#include "../../utils/toString.h"

namespace scl{
	namespace tools{
		namespace meta{
			template <class T, class = void>
			struct __toStringDefined : std::false_type{
			};

			template <class T>
			struct __toStringDefined<T, void_t<scl::utils::ToString<T>>> : std::true_type{
			};

			/**
			 * Determines whether or not ToString has been specialized for the given type
			 * @tparam T being the type to test upon
			 * @return TRUE if ToString<T> is defined, FALSE otherwise
			 */
			template <class T>
			inline constexpr bool defines_scl_to_string(){
				return __toStringDefined<T>::value;
			}
		}
	}
}