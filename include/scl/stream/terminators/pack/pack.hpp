#pragma once

namespace scl{
	namespace stream{
		namespace terminators{
			/**
			 * @namespace scl::stream::terminators::pack
			 * A namespace for packing operations (getting a result as a container)
			 */
			namespace pack{}
		}
	}
}

#include <scl/stream/terminators/pack/toVector.h>
#include <scl/stream/terminators/pack/toSet.h>
