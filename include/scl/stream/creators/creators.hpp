#pragma once

#include <scl/stream/details/iterator/iterator.hpp>

namespace scl{
	namespace stream{
		/**
		 * @namespace scl::stream::creators
		 * Namespace for functions and utilities that create streams (lazy)
		 */
		namespace creators{
			/**
			 * @namespace scl::stream::creators::details
			 * Implementation details of creators
			 */
			namespace details{}
		}
	}
}

#include <scl/stream/creators/array.h>
#include <scl/stream/creators/stl_iterators.h>
#include <scl/stream/creators/stl_containers.h>
#include <scl/stream/creators/range.h>