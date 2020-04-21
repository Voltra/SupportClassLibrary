#pragma once
#include <scl/stream/details/deprecation.h>

namespace scl{
	namespace stream{
		/**
		 * @namespace scl::stream::operators
		 * Namespace for intermediate operations on streams (lazy)
		 */
		namespace operators{
			/**
			 * @namespace scl::stream::operators::details
			 * Namespace for implementation details of operators
			 */
			namespace details{}
		}
	}
}

#include <scl/stream/operators/map.h>
#include <scl/stream/operators/filter.h>
#include <scl/stream/operators/uniqueBy.h>
#include <scl/stream/operators/unique.h>