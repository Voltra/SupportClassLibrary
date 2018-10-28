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
				namespace prefixed{
					template <class T=std::string>
					inline std::function<decltype(std::cout)&(const T&)> log(realConst(char*) prefix){
						decltype(std::cout)* cout = &std::cout;
						return [=](const T& value) -> decltype(std::cout)&{
							return (*cout) << prefix << value << nl;
						};
					}

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