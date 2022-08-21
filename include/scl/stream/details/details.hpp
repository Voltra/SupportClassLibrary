#pragma once

namespace scl {
    namespace stream {
        /**
         * Implementation details of the Stream API
         */
        namespace details {}
    }
}

#include "./StlAdapter.h"
#include "./StreamIterator.h"
#include "./BaseStreamIterator.h"
#include "./OpStreamIterator.h"
#include "./EndStreamIterator.h"
#include "./StreamPayload.h"
