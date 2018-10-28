#pragma once

#include <memory>
#include <scl/tools/make/ptr.h>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class... Args>
			std::unique_ptr<T> unique(Args&&... args){
				return std::unique_ptr<T>{
					scl::tools::make::ptr<T>(std::forward<Args>(args)...)
				};
			}
		}
	}
}