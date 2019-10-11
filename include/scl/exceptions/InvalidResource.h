#pragma once

#include <scl/exceptions/RuntimeException.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when a resource is used without specialized traits
		 */
		struct InvalidResource : RuntimeException{
			using RuntimeException::RuntimeException;
			InvalidResource() : InvalidResource{"Resource does not specialize scl::async::with_traits"} {
			}

			realConst(char*) type() const noexcept override{
				return "scl::exceptions::InvalidResource";
			}
		};
	}
}