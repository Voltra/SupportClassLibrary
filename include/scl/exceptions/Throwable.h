#pragma once

#include <exception>
#include <string>
#include "../macros.h"
#include "../utils/toString.h"
#include "../tools/meta/enable_if.h"
#include "../tools/meta/is_base_of.h"

namespace scl{
	namespace exceptions{
		/**
		 * Base class of all throwable objects
		 */
		struct Throwable : public virtual std::exception{
			virtual realConst(char*) reason() const{
				return std::exception::what();
			}

			/**
			 * The name of this Throwable type
			 * @return a stirng literal containing the name of the Throwable type
			 */
			virtual realConst(char*) type() const noexcept = 0;
		};
	}

	namespace utils{
		/**
		 * Specialization of ToString for Throwable types
		 */
		template <>
		struct ToString<exceptions::Throwable>{
			std::string operator()(const exceptions::Throwable& t) const{
				return std::string{t.type()} + " : " + t.reason();
			}
		};
	}
}

/*std::ostream& operator<<(std::ostream& os, scl::exceptions::Throwable& t){
	return os << scl::utils::toString<scl::exceptions::Throwable>(t);
}*/

//#ifdef SCL_USE_IOSTREAM_EXTEND
//namespace std{
//	string to_string(const scl::exceptions::Throwable& t){
//		return std::string{t.type()} + " : " + t.reason();
//	}
//}
//#endif