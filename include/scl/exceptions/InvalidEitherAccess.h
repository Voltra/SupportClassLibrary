#pragma once
#include "../macros.h"

#if !SCL_NO_EXCEPTIONS

    #include "../meta/type_aliases/real_const.h"
    #include "./Exception.h"

namespace scl {
    namespace exceptions {
	struct InvalidEitherAccess : public Exception {
		using Exception::Exception;

		scl::meta::real_const_t<char*> type() const noexcept override {
		    return "scl::exceptions::InvalidEitherAccess";
		}

		/**
		 * Helper function that creates an InvaidEitherAccess when trying to access Lhs when
		 * Rhs is active
		 * @return the exception
		 */
		static InvalidEitherAccess leftWhenRight() {
		    return InvalidEitherAccess{
			"Tried to access Lhs member of Either<Lhs, Rhs> when Rhs was set"};
		}

		/**
		 * Helper function that creates an InvaidEitherAccess when trying to access Rhs when
		 * Lhs is active
		 * @return the exception
		 */
		static InvalidEitherAccess rightWhenLeft() {
		    return InvalidEitherAccess{
			"Tried to access Rhs member of Either<Lhs, Rhs> when Lhs was set"};
		}
	};
    }  // namespace exceptions
}  // namespace scl
#endif
