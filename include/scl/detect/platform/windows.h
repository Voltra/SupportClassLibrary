#pragma once

/**
 * @def SCL_PLATFORM_WIN64
 * Macro that detects whether or not the platform is a 64bit Windows
 */
#if defined(_WIN64) || defined(WIN64)
    #define SCL_PLATFORM_WIN64 true
#else
    #define SCL_PLATFORM_WIN64 false
#endif

/**
 * @def SCL_PLATFORM_WIN32
 * Macro that detects whether or not the platform is a 32bit Windows
 */
#if defined(_WIN32) || defined(WIN32)
    #define SCL_PLATFORM_WIN32 (!SCL_PLATFORM_WIN64)
#else
    #define SCL_PLATFORM_WIN32 false
#endif

/**
 * @def SCL_PLATFORM_WINDOWS
 * Macro that detects whether or not the platform is Windows
 */
#if defined(SCL_PLATFORM_WIN32) || defined(SCL_PLATFORM_WIN64)
    #define SCL_PLATFORM_WINDOWS true
#else
    #define SCL_PLATFORM_WINDOWS false
#endif

namespace scl {
    namespace detect {
        namespace platform {
            /**
             * @var scl::detect::platform::win64
             * Checks whether or not the platform is a 64bit Windows
             */
            constexpr bool win64 = SCL_PLATFORM_WIN64;

            /**
             * @var scl::detect::platform::win32
             * Checks whether or not the platform is a 32bit Windows
             */
            constexpr bool win32 = SCL_PLATFORM_WIN32;

            /**
             * @var scl::detect::platform::windows
             * Checks whether or not the platform is windows
             */
            constexpr bool windows = SCL_PLATFORM_WINDOWS;
        }  // namespace platform
    }      // namespace detect
}  // namespace scl