#pragma once

#include <scl/macros.h>
#include <scl/utils/Optional.h>
#include <scl/utils/Either.h>
#include <scl/utils/Result.h>
#include <scl/utils/Any.h>
#include <scl/utils/Placeholder.h>
#include <scl/tools/make/make.hpp>
#include <scl/alias/alias.hpp>
#include <scl/exceptions/exceptions.hpp>

#ifndef SCL_CPP20
	#include <scl/stream/stream.hpp>
#endif

namespace scl{
	/**
	 * @namespace scl::prelude
	 * Namespace that groups together a lot of useful and often used features in one place
	 * @code
	 * #include <scl/prelude/prelude.hpp>
	 * using namespace scl::prelude;
	 * @endcode
	 */
	namespace prelude{
		namespace make = scl::tools::make;

		using scl::utils::Any;
		using scl::utils::Optional;
		using scl::utils::Either;
		using scl::utils::Result;

		using namespace scl::utils::placeholder;
		using namespace scl::alias;
		using namespace scl::exceptions;
#ifndef SCL_CPP20
		using namespace scl::stream::creators;
		using namespace scl::stream::operators;
		using namespace scl::stream::terminators;
#endif
	}
}