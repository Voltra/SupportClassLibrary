#pragma once

namespace scl{
	namespace tools{
		/**
		 * @namespace scl::tools::iostream
		 * Namespace for the iostream oriented tools
		 */
		namespace iostream{}
	}
}

#include <scl/tools/iostream/nl.h>
#ifdef SCL_USE_IOSTREAM_EXTEND
#include <scl/tools/iostream/extend.h>
#endif
#include <scl/tools/iostream/toString.h>
#include <scl/tools/iostream/log/log.hpp>