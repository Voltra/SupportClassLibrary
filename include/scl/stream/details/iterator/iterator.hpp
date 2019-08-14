#pragma once

namespace scl{
	namespace stream{
		namespace details{
			/**
			 * @namespace scl::stream::details::iterator
			 * Iterators abstract classes
			 */
			namespace iterator{}
		}
	}
}

#include <scl/stream/details/iterator/StlAdapter.h>
#include <scl/stream/details/iterator/StreamIterator.h>
#include <scl/stream/details/iterator/EndStreamIterator.h>
#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/stream/details/iterator/OpStreamIterator.h>