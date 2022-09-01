#include <array>
#include <iostream>
#include <scl/scl.hpp>
#include <string>

using namespace scl::prelude;

int main(SCL_MAYBE_UNUSED int argc, SCL_MAYBE_UNUSED char** argv) {
    std::array<int, 5> arr{1, 2, 3, 4, 5};

    streamFrom(arr) | map([](int x) {
	return x * 2;
    }) | forEach([](int x) {
	std::cout << x << '\n';
    });

    make::left<int, double>(1)
	.flatMapLeft([](int val) {
	    return make::left<std::string, double>(std::to_string(val));
	})
	.mapLeft([](std::string&& str){
	    return "<" + std::move(str) + ">";
	})
	.doIfLeft([](std::string&& val) {
	    std::cout << "Either flatMapped: " << val << '\n';
	});

    return 0;
}
