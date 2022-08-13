#pragma once
#include "./apple.h"
#include "./android.h"

/**
 * @def SCL_PLATFORM_LINUX
 * Macro that detects whether or not the platform is a Linux platform (that is neither android or apple)
 */
#if defined(__linux__)
    #define SCL_PLATFORM_LINUX ((!SCL_PLATFORM_APPLE) && (!SCL_PLATFORM_ANDROID))
#else
    #define SCL_PLATFORM_LINUX false
#endif

namespace scl{
    namespace detect{
        namespace platform{
            /**
			 * @var scl::detect::platform::linux
			 * Checks whether or not the platform is a Linux platform (that is neither android or apple)
             */
            constexpr bool linux = SCL_PLATFORM_LINUX;
        }
	}
}