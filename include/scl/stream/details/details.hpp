#pragma once

namespace scl {
    namespace stream {
        /**
         * Implementation details of the Stream API
         */
        namespace details {}
    }  // namespace stream
}  // namespace scl

#include "./BaseStreamIterator.h"
#include "./EndStreamIterator.h"
#include "./OpStreamIterator.h"
#include "./StlAdapter.h"
#include "./StreamIterator.h"
#include "./StreamPayload.h"
