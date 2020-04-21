#pragma once
#include <scl/stream/details/deprecation.h>

namespace scl{
	namespace stream{
		namespace details{
			/**
			 * @deprecated Iterators now use Optional instead of Either
			 */
			struct InvalidValue{};
		}
	}
}