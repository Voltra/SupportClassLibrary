#pragma once
#include <scl/exceptions/RuntimeException.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when trying to access uninitialized memory
		 */
		struct UninitializedMemoryAccess : public RuntimeException{
			using RuntimeException::RuntimeException;

			UninitializedMemoryAccess() : UninitializedMemoryAccess("Trying to access uninitialized memory") {
			}

			realConst(char*) type() const noexcept override {
				return "scl::exceptions::UninitializedMemoryAccess";
			}
		};
	}
}