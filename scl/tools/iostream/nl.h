#pragma once

#include <iostream>

namespace scl{
	namespace tools{
		namespace iostream{
			inline std::ostream& nl(std::ostream& os){
				return os << '\n';
			}
		}
	}
}