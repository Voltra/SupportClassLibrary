#pragma once

#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/tools/meta/type_check.h>
#include <scl/macros.h>

namespace scl{
	namespace tools {
		namespace meta{
			/**
			 * Get the appropriate const version of a type
			 * @tparam T the type which will be const qualified
			 */
			template <class T, class = void>
			struct real_const{
				using type = const decay_t<remove_cv_ref_t<T>>;
			};

			/*template <class T>
			struct real_const<T*, void>{
				using type = realConst(T*);
			};*/

			template <class T>
			struct real_const<T, enable_if_t<
				is_pointer<remove_cv_ref_t<T>>()
			>>{
				using type = realConst(remove_cv_ref_t<T>);
			};


			template <class T>
			using real_const_t = typename real_const<T>::type;
		}
	}
}