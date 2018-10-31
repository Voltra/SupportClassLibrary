#pragma once

#include <string>
#include <iostream>
#include <functional>
#include <scl/tools/iostream/nl.h>
#include <scl/macros.h>

namespace scl{
	namespace tools{
		namespace iostream{
			namespace log{
				/**
				 * @namespace scl::tools::iostream::log::prefixed
				 * Logging funtions with prefixes
				 */
				namespace prefixed{
					/**
					 * Log for the given type on stdout w/ a prefix and end with a line break
					 * @tparam T being the type of objects to log
					 * @param prefix being the prefix
					 * @return a logging function that uses the prefix
					 */
					template <class T=std::string>
					inline std::function<decltype(std::cout)&(const T&)> log(realConst(char*) prefix){
						decltype(std::cout)* cout = &std::cout;
						return [=](const T& value) -> decltype(std::cout)&{
							return (*cout) << prefix << value << nl;
						};
					}

					/**
					 * Log for the given type on stderr w/ a prefix and end with a line break
					 * @tparam T being the type of objects to log
					 * @param prefix being the prefix
					 * @return a logging function that uses the prefix
					 */
					template <class T=std::string>
					inline std::function<decltype(std::cerr)&(const T&)> error(realConst(char*) prefix){
						decltype(std::cerr)* cerr = &std::cerr;
						return [=](const T& value) -> decltype(std::cerr)&{
							return (*cerr) << prefix << value << nl;
						};
					}
				}
			}
		}
	}
}