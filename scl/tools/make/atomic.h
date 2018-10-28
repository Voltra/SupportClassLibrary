#pragma once

#include <utility>
#include <atomic>
#include <scl/tools/make/from.h>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class... Args>
			inline std::atomic<T> atomic(Args&&... args){
				return std::atomic<T>{make::from(std::forward<Args>(args)...)};
			}
		}
	}
}