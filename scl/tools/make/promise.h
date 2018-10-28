#pragma once

#include <utility>
#include <future>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class... Args>
			inline std::promise<T> promise(Args&&... args){
				auto ret = std::promise<T>{};
				ret.set_value(T{std::forward<Args>(args)...});
				return ret;
			}
		}
	}
}