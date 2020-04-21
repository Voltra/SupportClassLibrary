#pragma once
#include <scl/stream/details/deprecation.h>

namespace scl{
	namespace stream{
		/**
		 * @namespace scl::stream::terminators
		 * Namespace for stream terminal/final operations (eager)
		 */
		namespace terminators{
			/**
			 * @namespace scl::stream::terminators::details
			 * Namespace for implementation details of terminators
			 */
			namespace details{}
		}
	}
}

#include <scl/stream/terminators/pack/pack.hpp>
#include <scl/stream/terminators/forEach.h>