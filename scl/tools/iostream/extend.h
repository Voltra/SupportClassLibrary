#pragma once

#include <iostream>
#include <string>
#include <scl/tools/meta/meta.hpp>

namespace scl{
	namespace tools{
		namespace iostream{
			template <class T, class = meta::enable_if_t<
				!meta::is_convertible<T, std::string>()
				&& !meta::is_convertible<T, char>()
			>>
			inline std::ostream& operator<<(std::ostream& os, T& obj){
				return os << std::to_string(obj);
			}

			template <class T, class = meta::enable_if_t<
				!meta::is_convertible<T, std::string>()
				&& !meta::is_convertible<T, char>()
			>>
			inline std::ostream& operator<<(std::ostream& os, const T& obj){
				return os << std::to_string(obj);
			}

			template <class T, class = meta::enable_if_t<
				!meta::is_convertible<T, std::string>()
				&& !meta::is_convertible<T, char>()
			>>
			inline std::ostream& operator<<(std::ostream& os, T&& obj){
				return os << std::to_string(obj);
			}
		}
	}
}

//Note: to enable these operators, you must be in/use the namespace scl::tools::iostream