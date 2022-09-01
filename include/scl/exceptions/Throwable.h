#pragma once
#include "../macros.h"

#if !SCL_NO_EXCEPTIONS
    #include <string>

    #include "../meta/type_aliases/real_const.h"

namespace scl {
    namespace exceptions {
	/**
	 * Base class of all throwable objects
	 */
	struct Throwable : public virtual std::exception {
		/**
		 * Retrieve the reason
		 * @return the reason as a const char* const
		 */
		virtual scl::meta::real_const_t<char*> reason() const {
		    return std::exception::what();
		}

		/**
		 * The name of this Throwable type
		 * @return a string literal containing the name of the Throwable type
		 */
		virtual scl::meta::real_const_t<char*> type() const noexcept = 0;
	};
    }  // namespace exceptions
}  // namespace scl

namespace std {
    std::string to_string(const scl::exceptions::Throwable& throwable) {
	return std::string{throwable.type()} + " : " + throwable.reason();
    }
}  // namespace std
#endif