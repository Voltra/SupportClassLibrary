#pragma once
#include <scl/exceptions/Exception.h>
#include <scl/macros.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when attempting to access the wrong type stored in an scl::utils::Either
		 */
		class InvalidResultAccess : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::InvalidResultAccess";
				}

				/**
				 * Helper function that creates an InvalidResultAccess when trying to access Ok when Err
				 * @return the exception
				 */
				static InvalidResultAccess okWhenErr(){
					return InvalidResultAccess{"Tried to access value member of an error Result<T, E>"};
				}

				/**
				 * Helper function that creates an InvalidResultAccess when trying to access Err when Ok is active
				 * @return the exception
				 */
				static InvalidResultAccess errWhenOk(){
					return InvalidResultAccess{"Tried to access error member of a success Result<T, E>"};
				}
		};
	}
}