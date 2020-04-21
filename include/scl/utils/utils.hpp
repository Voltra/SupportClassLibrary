#pragma once

namespace scl{
	/**
	 * @namespace scl::utils
	 * Utility classes (and rare functions)
	 */
	namespace utils{
		/**
		 * @namespace scl::utils::details
		 * Implementation details for the utility classes
		 */
		namespace details{}
	}
}

#include <scl/utils/toString.h>
#include <scl/utils/Any.h>
#include <scl/utils/RawStorage.h>
#include <scl/utils/Optional.h>
#include <scl/utils/Either.h>
#include <scl/utils/Result.h>
#include <scl/utils/Placeholder.h>