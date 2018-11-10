#pragma once

#pragma once

#include <string>
#include <iostream>
#include <functional>
#include "../nl.h"
#include "../../../macros.h"

namespace scl{
	namespace tools{
		namespace iostream{
			namespace log{
				/**
				 * @namespace scl::tools::iostream::log::surrounded
				 * Logging utils but with both prefix and suffix
				 */
				namespace surrounded{
					/**
					 * Log on stdout with a prefix and a suffix, end with a line break
					 * @tparam T being the type of objects to log
					 * @param prefix being the logging prefix
					 * @param suffix being the logging suffix
					 * @return a logging function that uses both prefix and suffix
					 */
					template <class T=std::string>
					inline std::function<decltype(std::cout)&(const T&)> log(realConst(char*) prefix, realConst(char*) suffix){
						decltype(std::cout)* cout = &std::cout;
						return [=](const T& value) -> decltype(std::cout)&{
							return (*cout) << prefix << value << suffix << nl;
						};
					}

					/**
					 * Log on stderr with a prefix and a suffix, end with a line break
					 * @tparam T being the type of objects to log
					 * @param prefix being the logging prefix
					 * @param suffix being the logging suffix
					 * @return a logging function that uses both prefix and suffix
					 */
					template <class T=std::string>
					inline std::function<decltype(std::cerr)&(const T&)> error(realConst(char*) prefix, realConst(char*) suffix){
						decltype(std::cerr)* cerr = &std::cerr;
						return [=](const T& value) -> decltype(std::cerr)&{
							return (*cerr) << prefix << value << suffix <<  nl;
						};
					}
				}
			}
		}
	}
}