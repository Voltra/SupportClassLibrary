#pragma once

#include <iostream>
#include <utility>
#include <scl/tools/iostream/nl.h>

namespace scl{
	namespace tools{
		namespace iostream{
			namespace log{
				/**
				 * Output a value on stdout and terminate with a line break
				 * @return a reference to std::cout
				 */
				template <class T=std::string>
				inline decltype(std::cout)& log(T&& value){
					return std::cout << std::forward<T>(value) << nl;
				}

				/**
				 * Output a value on stderr and terminate with a line break
				 * @return a reference to std::cerr
				 */
				template <class T=std::string>
				inline decltype(std::cerr)& error(T&& value){
					return std::cerr << std::forward<T>(value) << '\n';
				}
			}
		}
	}
}