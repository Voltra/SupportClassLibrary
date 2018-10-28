#pragma once

namespace scl{
	namespace tools{
		namespace iostream{
			namespace log{}
		}
	}
}

#include <scl/tools/iostream/log/basic.h>
#include <scl/tools/iostream/log/prefixed.h>
#include <scl/tools/iostream/log/surrounded.h>

namespace scl{
	namespace tools{
		namespace iostream{
			namespace log{
				namespace wrap = surrounded;
			}
		}
	}

	namespace console = tools::iostream::log;
	namespace cli = console;
}