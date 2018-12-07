#pragma once

#include <scl/exceptions/Exception.h>
#include <scl/macros.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when attempting to access the value of an empty scl::utils::Optional
		 */
		class EmptyOptionalAccess : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::EmptyOptionalAccess";
				}

				/**
				 * Constructor that uses a default error message
				 */
				EmptyOptionalAccess() : EmptyOptionalAccess("Tried to access the value of an empty Optional"){}
		};
	}
}