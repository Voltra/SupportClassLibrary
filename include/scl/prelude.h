#pragma once
#include "./alias/alias.hpp"
#include "./detect/detect.hpp"
#include "./exceptions/exceptions.hpp"
#include "./make/make.hpp"
#include "./stream/stream.hpp"
#include "./utils/utils.hpp"

namespace scl {
    /**
     * @namespace scl::prelude
     * Namespace that groups together a lot of useful and often used features in one place
     * @code
     * 	#include <scl/prelude.h>
     * 	using namespace scl::prelude;
     * @endcode
     */
    namespace prelude {
	namespace make = scl::make;
	namespace version = scl::detect::version;

	using namespace scl::alias;
#if !SCL_NO_EXCEPTIONS
	using namespace scl::exceptions;
#endif
	using namespace scl::stream;
	using namespace scl::stream::creators;
	using namespace scl::stream::operators;
	using namespace scl::stream::terminators;

	using scl::utils::Either;
	using scl::utils::exchange;
	using scl::utils::invoke;
	using scl::utils::none;
	using scl::utils::Optional;
	using scl::utils::StringView;
    }  // namespace prelude
}  // namespace scl