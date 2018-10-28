#include <scl/scl.hpp>

using namespace scl::tools;
using namespace scl::tools::iostream;

int main(){
	auto ptr = make::unique<int>(42);
	std::cout << *ptr << nl;
	return 0;
}