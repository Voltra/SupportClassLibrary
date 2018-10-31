#pragma once

#include <iostream>

namespace scl{
	namespace tools{
		namespace iostream{
			/**
			 * An alternative for std::endl that actually does what you assumed std::endl was doing
			 * @param os being the ostream to output the lf character to
			 * @return a reference to the ostream
			 */
			inline std::ostream& nl(std::ostream& os){
				return os << '\n';
			}
		}
	}
}