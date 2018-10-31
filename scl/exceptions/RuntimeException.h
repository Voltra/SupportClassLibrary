#pragma once

#include <scl/exceptions/Throwable.h>
#include <scl/macros.h>
#include <stdexcept>
#include <utility>

namespace scl{
	namespace exceptions{
		/**
		 * Base class for exceptions that could never be checked at compile-time
		 */
		class RuntimeException : public Throwable, public std::runtime_error{
			protected:
				realConst(char*) _reason = "";

			public:
				template <class T>
				RuntimeException(T&& reason) : _reason{std::forward<T>(reason)} {
				}

				realConst(char*) reason() const override{
					return this->_reason;
				}

				const char* what() const noexcept override{
					return this->reason();
				}

				realConst(char*) type() const noexcept override{
					return "scl::exceptions::RuntimeException";
				}
		};
	}
}