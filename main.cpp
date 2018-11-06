#include <sstream>
#include <string>
#include <memory>
#include <vector>
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
void ostreamStringChecks(){
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
//	require(Iterator<int*>{});
	require(SwappableWith<int, int>{});
	std::cout << "None: " << none << nl;
	std::cout << "Optional<float>: " << make::optional<int>(3).mapTo<float>([](const int& i){ return 0.14f + i; }).orElse(42.f) << nl;
	auto any = make::any<stringLiteral>("coucou");
	std::cout << "Any<stringLiteral>: " << any.as<stringLiteral>() << nl;
	std::cout << "Either<int, stringLiteral>.left: " << Either<int, stringLiteral>::Left(42).getLeft() << nl;
	auto ptr = make::unique<int>(answerToLife<int>());
	auto log = scl::cli::wrap::log<int>("$> ", " <$");
	log(*ptr);

	std::cout << "addr: " << ptr << nl;
	return 0;
}