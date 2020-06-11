#pragma once

#include <scl/utils/Placeholder.h>
#include <scl/utils/Result.h>

namespace scl{
	namespace tools{
		namespace make{
			template <class T, class E = scl::utils::Placeholder>
			scl::utils::Result<T, E> ok(const T& value){
				return scl::utils::Result<T, E>::Ok(value);
			}

			template <class T, class E>
			scl::utils::Result<T, E> err(const E& error){
				return scl::utils::Result<T, E>::Err(error);
			}

		}
	}
}