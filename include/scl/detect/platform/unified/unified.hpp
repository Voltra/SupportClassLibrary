#pragma once

namespace scl{
	namespace detect{
		namespace platform{
			/**
			 * @namespace scl::detect::platform::unified
			 * Platform detection but in a unified way (e.g. if android then linux, if ios then unix)
			 */
			namespace unified{}
		}
	}
}

#include <scl/detect/platform/unified/windows.h>
#include <scl/detect/platform/unified/posix.h>

// Clean up macros
#include <scl/detect/platform/unified/unified.clean_macros.h>