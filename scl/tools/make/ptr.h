#pragma once

#include <utility>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class... Args>
			inline T* ptr(Args&&... args){
				return new T(std::forward<Args>(args)...);
			}
		}
	}
}