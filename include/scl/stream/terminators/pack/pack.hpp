#pragma once
#include <scl/stream/details/deprecation.h>

namespace scl{
	namespace stream{
		namespace terminators{
			/**
			 * @namespace scl::stream::terminators::pack
			 * A namespace for packing operations (getting a result as a container)
			 */
			namespace pack{
				/**
				 * @namespace scl::stream::terminators::pack::details
				 * Implementation details for packing operations
				 */
				namespace details{}
			}
		}
	}
}

#include <scl/stream/terminators/pack/toVector.h>
#include <scl/stream/terminators/pack/toSet.h>
