#pragma once

#include <iostream>
#include <scl/tools/iostream/nl.h>

namespace scl{
	namespace tools{
		namespace iostream{
			namespace log{
				template <class T=std::string>
				inline decltype(std::cout)& log(T value){
					return std::cout << value << nl;
				}

				template <class T=std::string>
				inline decltype(std::cerr)& error(T value){
					return std::cerr << value << '\n';
				}
			}
		}
	}
}