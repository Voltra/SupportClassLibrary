#include <sstream>
#include <string>
#include <memory>

namespace std{
	template <class T>
	inline string to_string(T* ptr){ stringstream ss; ss << ptr; return ss.str(); }

	template <class T>
	inline string to_string(unique_ptr<T>& uptr){ return to_string(uptr.get()); }
}

#include <scl/scl.hpp>
using namespace scl::tools;
using namespace scl::tools::iostream;

int main(){
	auto ptr = make::unique<int>(42);
	auto log = scl::cli::wrap::log<int>("$> ", " <$");
	log(*ptr);
	std::cout << "addr: " << ptr << nl;
	return 0;
}