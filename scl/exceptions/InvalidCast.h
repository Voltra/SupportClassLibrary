#pragma once

#include <scl/exceptions/Exception.h>
#include <scl/macros.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when a conversion is impossible
		 */
		class InvalidCast : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::InvalidCast";
				}
		};
	}
}