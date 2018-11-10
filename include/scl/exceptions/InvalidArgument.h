#pragma once

#include "Exception.h"
#include "../macros.h"

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when passing an argument that does not respect the preconditions
		 */
		class InvalidArgument : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::InvalidArgument";
				}
		};
	}
}