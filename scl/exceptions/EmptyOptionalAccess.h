#pragma once

#include <scl/exceptions/Exception.h>
#include <scl/macros.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when attempting to access the value of an empty optional
		 */
		class EmptyOptionalAccess : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::EmptyOptionalAccess";
				}
		};
	}
}