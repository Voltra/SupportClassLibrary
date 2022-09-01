#pragma once
#include "../macros.h"

#if !SCL_NO_EXCEPTIONS
    #include "../meta/type_aliases/real_const.h"
    #include "./Throwable.h"

namespace scl {
    namespace exceptions {
	class RuntimeException : public Throwable {
	    protected:
		/**
		 * @var reason_
		 * The error message that comes with this Exception
		 */
		scl::meta::real_const_t<char*> reason_;

	    public:
		/**
		 * Construct an scl::exceptions::Exception from an error message
		 * @tparam T being the reason's type
		 * @param reason being the error message
		 */
		explicit RuntimeException(const char* reason) : reason_{reason} {}

		/**
		 * Retrieve the reason
		 * @return the reason as a const char* const
		 */
		scl::meta::real_const_t<char*> reason() const override {
		    return this->reason_;
		}

		/**
		 * Override that uses the user defined reason
		 * @return the reason
		 */
		const char* what() const noexcept override {
		    return this->reason();
		}

		/**
		 * A string describing the name of the exception class
		 * @return the fully qualified name of this class
		 */
		scl::meta::real_const_t<char*> type() const noexcept override {
		    return "scl::exceptions::RuntimeException";
		}
	};
    }  // namespace exceptions
}  // namespace scl
#endif