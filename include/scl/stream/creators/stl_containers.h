#pragma once

#include <scl/stream/creators/stl_iterators.h>
#include <scl/macros.h>
#include <scl/tools/meta/meta.hpp>

namespace scl{
	namespace stream{
		namespace creators{
			/**
			 * Create a stream from a container
			 * @tparam C being the container type
			 * @tparam It being the computed iterator type
			 * @param container being the container to construct from
			 * @return a stream that uses the container as data source
			 */
			template <class C, class It = typename C::iterator>
			Stream<typename details::FromStlIterator<It>::value_type> streamFrom(C& container){
				return streamFrom<It>(std::begin(container), std::end(container));
			}
		}
	}
}