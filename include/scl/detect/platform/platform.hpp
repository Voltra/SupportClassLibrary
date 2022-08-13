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

#include "./windows.h"
#include "./android.h"
#include "./apple.h"
#include "./linux.h"
#include "./unix.h"
#include "./posix.h"
