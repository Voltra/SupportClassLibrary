#pragma once

#include <memory>
#include <scl/tools/make/ptr.h>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class... Args>
			std::shared_ptr<T> shared(Args&&... args){
				return std::shared_ptr<T>{
					scl::tools::make::ptr(std::forward<Args>(args)...)
				};
			}
		}
	}
}