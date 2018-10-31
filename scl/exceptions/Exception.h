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
				realConst(char*) _reason = "";

			public:
				template <class T>
				Exception(T&& reason) : _reason{std::forward<T>(reason)} {
				}

				realConst(char*) reason() const override{
					return this->_reason;
				}

				const char* what() const noexcept override{
					return this->reason();
				}

				realConst(char*) type() const noexcept override{
					return "scl::exceptions::Exception";
				}
		};
	}
}