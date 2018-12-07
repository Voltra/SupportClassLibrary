#pragma once

#include <scl/exceptions/Exception.h>
#include <scl/macros.h>

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when attempting to access the wrong type stored in an scl::utils::Either
		 */
		class InvalidEitherAccess : public Exception{
			public:
				using Exception::Exception;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::InvalidEitherAccess";
				}

				/**
				 * Helper function that creates an InvaidEitherAccess when trying to access Lhs when Rhs is active
				 * @return the exception
				 */
				static InvalidEitherAccess leftWhenRight(){
					return InvalidEitherAccess{"Tried to access Lhs member of Either<Lhs, Rhs> when Rhs was set"};
				}

				/**
				 * Helper function that creates an InvaidEitherAccess when trying to access Rhs when Lhs is active
				 * @return the exception
				 */
				static InvalidEitherAccess rightWhenLeft(){
					return InvalidEitherAccess{"Tried to access Rhs member of Either<Lhs, Rhs> when Lhs was set"};
				}
		};
	}
}