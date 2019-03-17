#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include "main.h"

#include <scl/scl.hpp>
using namespace scl::tools;
using namespace scl::tools::iostream;
using namespace scl::concepts;
using namespace scl::exceptions;
using namespace scl::utils;

using namespace scl::stream;
using namespace scl::stream::creators;
using namespace scl::stream::operators;
using namespace scl::stream::terminators;

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

float threeToPi(const int& i){ return 0.14f + i; }

template <class T>
void logt(const T& t){
	static auto l = scl::cli::wrap::log<T>("%> ", " <%");
	l(t);
}

void logf(const float& f){
	logt<float>(f);
}

void logs(const std::string& s){
	logt<std::string>(s);
}

struct Person{
	std::string name;

	template <class T>
	Person(T&& name) : name{std::forward<T>(name)}{
	}

	std::string getName() const{ return this->name; }
};

int main(){
	require(Iterator<int*>{});
	require(SwappableWith<int, int>{});

	std::cout << "None: " << none << nl;
	std::cout << "Optional<float>: " << make::optional<int>(3).mapTo<float>(threeToPi).orElse(42.f) << nl;

	auto any = make::any<stringLiteral>("coucou");
	std::cout << "Any<stringLiteral>: " << any.as<stringLiteral>() << nl;

	std::cout << "Either<int, stringLiteral>.left: " << Either<int, stringLiteral>::Left(42).getLeft() << nl;
	std::cout << nl;

	auto ptr = make::unique<int>(answerToLife<int>());
	auto log = scl::cli::wrap::log<int>("$> ", " <$");
	log(*ptr);
	std::cout << "addr: " << ptr << nl;
	std::cout << nl;

	std::vector<Person> p = {
		Person{"jean"},
		Person{"michel"},
		Person{"denis"}
	};

	auto res = streamFrom(p)
	| map(&Person::getName)
	| map_<std::string>([](const std::string& s){ return s + " 42"; })
	| pack::toVector();
	std::for_each(std::begin(res), std::end(res), logs);
	std::cout << nl;

	std::array<int, 4> v = {1,2,3,4};
	streamFrom(v)
	| map_<int, float>([](const int& i) -> float{ return i+1.14f; })
	| forEach(logt<float>);
	std::cout << nl;

	std::array<int, 2> arr = {1, 1};
	auto res2 = streamFrom(arr)
	| map_<int>([](const int& i){ return i+1; })
	| pack::toSet();
	std::for_each(std::begin(res2), std::end(res2), logt<int>);
	std::cout << nl;

	rangeTo(10)
	| map_<int>([](const int& i){ return i+2; })
	| forEach(logt<int>);
}