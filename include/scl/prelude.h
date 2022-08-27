#pragma once
#include "./alias/alias.hpp"
#include "./make/make.hpp"
#include "./exceptions/exceptions.hpp"
#include "./detect/detect.hpp"
#include "./utils/utils.hpp"
#include "./stream/stream.hpp"

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
        using namespace scl::stream;
        using namespace scl::stream::creators;
        using namespace scl::stream::operators;
        using namespace scl::stream::terminators;

        using scl::utils::exchange;
        using scl::utils::invoke;
        using scl::utils::Optional;
        using scl::utils::none;
    }
}  // namespace scl