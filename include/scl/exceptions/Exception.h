#pragma once

#include <scl/exceptions/Throwable.h>
#include <scl/macros.h>
#include <utility>

namespace scl{
	namespace exceptions{
		/**
		 * Base class for all regular exceptions
		 */
		class Exception : public Throwable{
			protected:
				/**
				 * @var _reason
				 * The error message that comes with this Exception
				 */
				realConst(char*) _reason = "";

			public:
				/**
				 * Construct an scl::exceptions::Exception from an error message
				 * @tparam T being the reason's type
				 * @param reason being the error message
				 */
				Exception(const char* reason) : _reason{reason} {
				}

				/**
				 * Retrieve the reason
				 * @return the reason as a const char* const
				 */
				realConst(char*) reason() const override{
					return this->_reason;
				}

				/**
				 * Override that uses the user defined reason
				 * @return the reason
				 */
				const char* what() const noexcept override{
					return this->reason();
				}

				/**
				 * A string describing the name of the exception class
				 * @return the fully qualified name of this class
				 */
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::Exception";
				}
		};
	}
}