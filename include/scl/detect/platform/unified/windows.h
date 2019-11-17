#pragma once
#include <scl/detect/platform/windows.h>

#ifdef SCL_PLATFORM_WINDOWS
	#define SCL_UPLATFORM_WIN32 true
#else
	#define SCL_UPLATFORM_WIN32 false
#endif

#define SCL_UPLATFORM_WIN64 SCL_PLATFORM_WIN64
#define SCL_UPLATFORM_WINDOWS SCL_UPLATFORM_WIN32

namespace scl{
	namespace detect{
		namespace platform{
			namespace unified{
				constexpr bool win32 = SCL_UPLATFORM_WIN32;
				constexpr bool win64 = SCL_UPLATFORM_WIN64;
				constexpr bool windows = SCL_UPLATFORM_WINDOWS;
			}
		}
	}
}