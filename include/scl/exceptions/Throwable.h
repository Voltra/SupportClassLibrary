#pragma once

#include <exception>
#include <string>
#include <scl/macros.h>
#include <scl/utils/toString.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/is_base_of.h>

namespace scl{
	namespace exceptions{
		/**
		 * Base class of all throwable objects
		 */
		struct Throwable : public virtual std::exception{
			/**
			 * Retrieve the reason
			 * @return the reason as a const char* const
			 */
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
		template <class T>
		struct ToString<T, META::enable_if_t<
			META::is_base_of<scl::exceptions::Throwable, T>()
		>>{
			std::string operator()(const T& t) const{
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