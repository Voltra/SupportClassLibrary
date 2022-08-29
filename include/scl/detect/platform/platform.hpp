#pragma once

namespace scl {
    namespace detect {
        /**
         * @namespace scl::detect::platform
         * Namespace for platform detection
         */
        namespace platform {}
    }  // namespace detect
}  // namespace scl

#include "./android.h"
#include "./apple.h"
#include "./linux.h"
#include "./posix.h"
#include "./unix.h"
#include "./windows.h"
