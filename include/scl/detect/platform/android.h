#pragma once

/**
 * @def SCL_PLATFORM_ANDROID
 * Macro that detects whether or not the platform is Android
 */
#if defined(__ANDROID__) && __ANDROID__
    #define SCL_PLATFORM_ANDROID true
#else
    #define SCL_PLATFORM_ANDROID false
#endif

namespace scl {
    namespace detect {
        namespace platform {
            constexpr bool android = SCL_PLATFORM_ANDROID;
        }
    }  // namespace detect
}  // namespace scl
