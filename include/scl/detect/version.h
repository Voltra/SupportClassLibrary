#pragma once
#include "../macros.h"

#define SCL_VERSION_CPP11 true

#ifdef SCL_CPP14
    #define SCL_VERSION_CPP14 true
#else
    #define SCL_VERSION_CPP14 false
#endif

#ifdef SCL_CPP17
    #define SCL_VERSION_CPP17 true
#else
    #define SCL_VERSION_CPP17 false
#endif

#ifdef SCL_CPP20
    #define SCL_VERSION_CPP20 true
#else
    #define SCL_VERSION_CPP20 false
#endif

namespace scl {
    namespace detect {
	/**
	 * @namespace scl::detect
	 * Namespace for detecting C++ versions
	 */
	namespace version {
	    constexpr bool cpp11 = SCL_VERSION_CPP11;
	    constexpr bool cpp14 = SCL_VERSION_CPP14;
	    constexpr bool cpp17 = SCL_VERSION_CPP17;
	    constexpr bool cpp20 = SCL_VERSION_CPP20;
	}  // namespace version
    }      // namespace detect
}  // namespace scl
