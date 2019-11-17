#pragma once
#include <scl/detect/platform/unix.h>

/**
 * @def SCL_PLATFORM_POSIX
 * Macro that detects whether or not the platform is a POSIX platform (that is not a Unix platform)
 */
#if defined(_POSIX_VERSION)
	#define SCL_PLATFORM_POSIX (!SCL_PLATFORM_UNIX)
#else
	#define SCL_PLATFORM_POSIX false
#endif

namespace scl{
	namespace detect{
		namespace platform{
			/**
			 * @var scl::detect::platform::posix
			 * Checks whether or not the platform is a POSIX platform (that is not a Unix platform)
			 */
			constexpr bool posix = SCL_PLATFORM_POSIX;
		}
	}
}