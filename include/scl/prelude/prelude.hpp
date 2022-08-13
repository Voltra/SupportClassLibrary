#pragma once
#include "../alias/alias.hpp"
#include "../make/make.hpp"
#include "../exceptions/exceptions.hpp"
#include "../detect/detect.hpp"

namespace scl {
    /**
     * @namespace scl::prelude
     * Namespace that groups together a lot of useful and often used features in one place
     * @code
     * #include <scl/prelude/prelude.hpp>
     * using namespace scl::prelude;
     * @endcode
     */
    namespace prelude {
        namespace make = scl::make;
        namespace version = scl::detect::version;

        using namespace scl::alias;
        using namespace scl::exceptions;
    }
}  // namespace scl