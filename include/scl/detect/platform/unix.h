#pragma once
#include <scl/detect/platform/linux.h>

/**
 * @def SCL_PLATFORM_UNIX
 * Macro that detects whether or not the platform is a Unix platform (that is not a Linux platform)
 */
#if defined(__unix__)
	#define SCL_PLATFORM_UNIX (!SCL_PLATFORM_LINUX)
#else
	#define SCL_PLATFORM_UNIX false
#endif

namespace scl{
	namespace detect{
		namespace platform{
			/**
			 * @var scl::detect::platform::unix
			 * Checks whether or not the platform is a Unix platform (that is not a Linux platform)
			 */
			constexpr bool unix = SCL_PLATFORM_UNIX;
		}
	}
}