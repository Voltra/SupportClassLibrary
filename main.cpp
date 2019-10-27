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
using namespace scl::utils::placeholder;

using namespace scl::stream;
using namespace scl::stream::creators;
using namespace scl::stream::operators;
using namespace scl::stream::terminators;

using namespace scl::http;

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

	Person(const Person&) = default;
	Person(Person&&) = default;
	Person& operator=(const Person&) = default;
	Person& operator=(Person&&) = default;


	explicit Person(const std::string& name) : name{name}{
	}

	std::string getName() const{ return this->name; }
};

void testsConcepts(){
	require(Iterator<int*>{});
	require(SwappableWith<int, int>{});
}

void testsMake(){
	std::cout << "None: " << none << nl;
	std::cout << "Optional<float>: " << make::optional<int>(3).mapTo<float>(threeToPi).orElse(42.f) << nl;

	auto any = make::any<stringLiteral>("coucou");
	std::cout << "Any<stringLiteral>: " << any.as<stringLiteral>() << nl;

	std::cout << "Either<int, stringLiteral>.left: " << Either<int, stringLiteral>::Left(42).getLeft() << nl;
	std::cout << nl;
}

void testsMakeUnique(){
	auto ptr = make::unique<int>(answerToLife<int>());
	auto log = scl::cli::wrap::log<int>("$> ", " <$");
	log(*ptr);
	std::cout << "addr: " << ptr << nl;
	std::cout << nl;
}

void testsStreamObj(){
	std::vector<Person> p = {
		Person{"jean"},
		Person{"michel"},
		Person{"denis"},
		Person{"2"}
	};

	auto res = streamFrom(p)
			   | uniqueBy(&Person::getName)
			   | map(&Person::getName)
			   | map(+[](const std::string& s){ return s + " 42"; })
			   | pack::toVector();
	std::for_each(std::begin(res), std::end(res), logs);
	std::cout << nl;
}

void testsVectorMapForEach(){
	std::array<int, 4> v = {1,2,3,4};
	streamFrom(v)
	| map(+[](const int& i) -> float{ return i+1.14f; })
	| forEach(logt<float>);
	std::cout << nl;
}

void testsStreamRange(){
	rangeTo(10)
	| map(+[](const int& i){ return i+2; })
	| forEach(logt<int>);

	rangeTo(5)
	| filter(+[](const int& i){ return i%2; })
	| forEach(logt<int>);
}

void testsStreamPackSet(){
	std::array<int, 2> arr = {1, 1};
	auto res2 = streamFrom(arr)
				| map(+[](const int& i){ return i+1; })
				| pack::toSet();
	std::for_each(std::begin(res2), std::end(res2), logt<int>);
	std::cout << nl;
}

void testsStream(){
	testsStreamObj();
	//testsVectorMapForEach();
	//testsStreamRange();
	//testsStreamPackSet();
}

void randomTests(){
	int a = 0;
	std::tie(_, a) = std::make_tuple(69, 42);
	std::cout << "OwO _ is working " << a << nl;

	std::cout << InvalidAnyCast{"OwO cool exceptions"} << nl;
}

void testHTTP(){
	std::cout << StatusCode::OK << nl;
}

int main(){
	//TODO: Fix references bug (undef ref / empty optional access)
	//testsStream();

	std::cout << asString(42) << nl;
//	std::cout << Optional<int>{42} << nl;
}