#pragma once
#include <scl/detect/platform/android.h>
#include <scl/detect/platform/apple.h>
#include <scl/detect/platform/linux.h>
#include <scl/detect/platform/unix.h>
#include <scl/detect/platform/posix.h>

#define SCL_UPLATFORM_ANDROID SCL_PLATFORM_ANDROID
#define SCL_UPLATFORM_APPLE SCL_PLATFORM_APPLE
#define SCL_UPLATFORM_IOS (SCL_PLATFORM_IOS || SCL_PLATFORM_IOS_SIMULATOR)
#define SCL_UPLATFORM_LINUX (SCL_PLATFORM_LINUX || SCL_UPLATFORM_APPLE || SCL_UPLATFORM_ANDROID)
#define SCL_UPLATFORM_UNIX (SCL_PLATFORM_UNIX || SCL_UPLATFORM_LINUX)
#define SCL_UPLATFORM_POSIX (SCL_PLATFORM_POSIX || SCL_UPLATFORM_UNIX)

namespace scl{
	namespace detect{
		namespace platform{
			namespace unified{
				constexpr bool android = SCL_UPLATFORM_ANDROID;
				constexpr bool apple = SCL_UPLATFORM_APPLE;
				constexpr bool ios = SCL_UPLATFORM_IOS;
				constexpr bool linux = SCL_UPLATFORM_LINUX;
				constexpr bool unix = SCL_UPLATFORM_UNIX;
				constexpr bool posix = SCL_UPLATFORM_POSIX;
			}
		}
	}
}