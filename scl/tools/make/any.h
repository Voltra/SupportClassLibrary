#pragma once

#include <scl/utils/Any.h>
#include <utility>

namespace scl{
	namespace tools{
		namespace make{
			/**
			 * Creates an any from the given type and arguments
			 * @tparam T being the stored value type
			 * @tparam Args being the arguments' types
			 * @param args being the arguments to pass to the value type's constructor
			 * @return an initialized Any whose value type is forced to be T
			 */
			template <class T, class... Args>
			scl::utils::Any any(Args&&... args){
				return scl::utils::Any(
					T{std::forward<Args>(args)...}
				);
			}
		}
	}
}