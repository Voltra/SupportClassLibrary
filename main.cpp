#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include "main.h"
#include <thread>

#include <scl/scl.hpp>
#include <fstream>

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
using namespace scl::async;

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

void testStreamFile(){
	std::ifstream file;
	file.open("./main.h");

	streamFrom(file)
	| forEach(+[](const std::string& str){
		std::cout << str << nl;
	});
}

int main(){ //TODO: Fix characters corruption?
	Channel<int> chan;

	/*auto& sender = chan.sender();
	auto& receiver = chan.receiver();
	std::thread([&]{ sender.send(42); }).detach();
	std::thread([&]{ std::cout << receiver.receive() << nl; }).join();*/

	std::thread([&]{ chan << 42.99; }).detach();
	std::thread([&]{
		Optional<int> i;
		chan >> i;
		std::cout << *i << nl;
	}).join();
}