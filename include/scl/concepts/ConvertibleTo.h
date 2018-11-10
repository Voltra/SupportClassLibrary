#pragma once

#include "../tools/meta/is_convertible.h"

namespace scl{
	namespace concepts{
		/**
		 * Conversion concept, a type From is convertible to another type To if meta::is_convertible<From, To>() is true
		 * @tparam From being the source type
		 * @tparam To being the destination type
		 */
		template <class From, class To>
		struct ConvertibleTo{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(
					meta::is_convertible<From, To>(),
					"ConvertibleTo<From, To>: Requirements for concept ConvertibleTo are not met."
				);
				return true;
			}
		};
	}
}