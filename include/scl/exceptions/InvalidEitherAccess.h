#pragma once

#include "Exception.h"
#include "../macros.h"

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when attempting to access the wrong type stored in an Either
		 */
		class InvalidEitherAccess : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::InvalidEitherAccess";
				}
		};
	}
}