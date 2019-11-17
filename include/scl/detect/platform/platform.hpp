#pragma once

namespace scl{
	namespace detect{
		/**
		 * @namespace scl::detect::platform
		 * Namespace for platform detection
		 */
		namespace platform{}
	}
}

#include <scl/detect/platform/windows.h>
#include <scl/detect/platform/android.h>
#include <scl/detect/platform/apple.h>
#include <scl/detect/platform/linux.h>
#include <scl/detect/platform/unix.h>
#include <scl/detect/platform/posix.h>
#include <scl/detect/platform/unified/unified.hpp>

// Clean up all macros if necessary
#include <scl/detect/platform/platform.clean_macros.h>