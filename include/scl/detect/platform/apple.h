#pragma once

/**
 * @def SCL_PLATFORM_APPLE
 * Macro that detects whether or not the platform is by Apple
 */
#if defined(__APPLE__) && __APPLE__
	#define SCL_PLATFORM_APPLE true
	#include "TargetConditionals.h"

	/**
	 * @def SCL_PLATFORM_IOS_SIMULATOR
	 * Macro that detects whether or not the platform is an emulator for IOS
	 */
	#if defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR
		#define SCL_PLATFORM_IOS_SIMULATOR true
	#else
		#define SCL_PLATFORM_IOS_SIMULATOR false
	#endif

	/**
	 * @def SCL_PLATFORM_IOS
	 * Macro that detects whether or not the platform is IOS (and not an emulator)
	 */
	#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
		#define SCL_PLATFORM_IOS (!SCL_PLATFORM_IOS_SIMULATOR)
	#else
		#define SCL_PLATFORM_IOS false
	#endif

	/**
	 * @def SCL_PLATFORM_OSX
	 * Macro that detects whether or not the platform is OSX (and not IOS)
	 */
	#if defined(TARGET_OS_MAC) && TARGET_OS_MAC
		#define SCL_PLATFORM_OSX (!SCL_PLATFORM_IOS)
	#else
		#define SCL_PLATFORM_OSX false
	#endif
#else
	#define SCL_PLATFORM_APPLE false
	#define SCL_PLATFORM_IOS_SIMULATOR false
	#define SCL_PLATFORM_IOS false
	#define SCL_PLATFORM_OSX false
#endif

/**
 * @def SCL_PLATFORM_MAC
 * Macro that detects whether or not the platform is a Mac
 */
#define SCL_PLATFORM_MAC SCL_PLATFORM_OSX

/**
 * @def SCL_PLATFORM_MAC
 * Macro that detects whether or not the platform is an iPhone
 */
#define SCL_PLATFORM_IPHONE SCL_PLATFORM_IOS

namespace scl{
	namespace detect{
		namespace platform{
			constexpr bool iosSimulator = SCL_PLATFORM_IOS_SIMULATOR;
			constexpr bool ios = SCL_PLATFORM_IOS;
			constexpr bool osx = SCL_PLATFORM_OSX;

			constexpr bool apple = SCL_PLATFORM_APPLE;
			constexpr bool mac = SCL_PLATFORM_MAC;
			constexpr bool iphone = SCL_PLATFORM_IPHONE;
		}
	}
}