#pragma once

#include "InvalidCast.h"
#include "../macros.h"

namespace scl{
	namespace exceptions{
		/**
		 * Exception class used when attempting to cast an any to the wrong type
		 */
		class InvalidAnyCast : public InvalidCast{
			public:
				using InvalidCast::InvalidCast;
				realConst(char*) type() const noexcept override{
					return "scl::exceptions::InvalidAnyCast";
				}
		};
	}
}