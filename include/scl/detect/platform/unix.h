#pragma once

/**
 * @def SCL_PLATFORM_UNIX
 * Macro that detects whether or not the platform is a Unix platform
 */
#if defined(__unix__)
    #define SCL_PLATFORM_UNIX true
#else
    #define SCL_PLATFORM_UNIX false
#endif

namespace scl {
    namespace detect {
	namespace platform {
	    /**
	     * @var scl::detect::platform::unix
	     * Checks whether or not the platform is a Unix platform
	     */
	    constexpr bool unix = SCL_PLATFORM_UNIX;
	}  // namespace platform
    }      // namespace detect
}  // namespace scl