#pragma once

#include <utility>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class... Args>
			inline T from(Args&&... args){
				return T{std::forward<Args>(args)...};
			}
		}
	}
}