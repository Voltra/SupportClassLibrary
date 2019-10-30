#pragma once
#include <scl/exceptions/Exception.h>

namespace scl{
	namespace exceptions{
		struct NonNullViolation : Exception{
			using Exception::Exception;

			NonNullViolation() : NonNullViolation("Got nullptr on a variable that is expected to be non null"){}

			realConst(char*) type() const noexcept override{
				return "scl::exceptions::NonNullViolation";
			}
		};
	}
}
