#include <sstream>
#include <string>
#include <memory>
#include "main.h"

#include <scl/scl.hpp>
using namespace scl::tools;
using namespace scl::tools::iostream;
using namespace scl::concepts;
using namespace scl::exceptions;
using namespace scl::utils;

template <class T>
T answerToLife(){
	require(Same<T, int>{});
	return 42;
}

class X{};

namespace scl{
	namespace utils{
		template <>
		struct ToString<X>{
			std::string operator()(const X&) const{
				return std::to_string(42);
			}
		};
	}
}

template <class T>
void defines(){
	std::boolalpha(std::cout);
	std::cout << "Defines ostream << : " << meta::defines_ostream_operator<T>() << nl;
	std::cout << "Defines scl::utils::ToString : " <<  meta::defines_scl_to_string<T>() << nl;
	std::cout << "Defines std::to_string : " <<  meta::defines_std_to_string<T>() << nl;
	std::cout << "Convertible to char : " <<  meta::is_convertible<T, char>() << nl;
	std::cout << "Convertible to char* : " <<  meta::is_convertible<T, stringLiteral>() << nl;
	std::cout << "Convertible to std::string : " <<  meta::is_convertible<T, std::string>() << nl;
	std::noboolalpha(std::cout);
}

int main(){
	std::cout << none << nl;
	auto ptr = make::unique<int>(answerToLife<int>());
	auto log = scl::cli::wrap::log<int>("$> ", " <$");
	log(*ptr);

	std::cout << "addr: " << ptr << nl;
	return 0;
}