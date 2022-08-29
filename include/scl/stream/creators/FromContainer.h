#pragma once
#include <utility>

#include "../../macros.h"
#include "../../meta/meta.hpp"
#include "../Stream.h"
#include "./FromIterators.h"

namespace scl {
    namespace stream {
        namespace creators {
            /**
             * Create a stream from a container
             * @tparam C being the container type
             * @tparam It being the computed iterator type
             * @param container being the container to construct from
             * @return a stream that uses the container as data source
             */
            template <class C, class It = typename C::iterator,
                      class = scl::meta::enable_if_t<scl::meta::is_iterator<It>()>>
            auto streamFrom(C& container)
                -> SCL_RETURN(streamFrom<It>(std::begin(container), std::end(container)))
        }  // namespace creators
    }      // namespace stream
}  // namespace scl