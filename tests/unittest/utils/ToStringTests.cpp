#include <gtest/gtest.h>
#include <scl/utils/toString.h>
#include <scl/tools/meta/meta.hpp>
#include <scl/macros.h>

#include <string>

using namespace scl::utils;

TEST(ToStringTests, StringTypeDefineToString){
	static_assert(META::exists<ToString<char, void>>(), "toString<char> is not defined");
	static_assert(META::exists<ToString<char*, void>>(), "toString<char*> is not defined");
	static_assert(META::exists<ToString<const char*, void>>(), "toString<const char*> is not defined");
	static_assert(META::exists<ToString<const char* const, void>>(), "toString<const char*> is not defined");
	static_assert(META::exists<ToString<std::string, void>>(), "toString<const char*> is not defined");
	SUCCEED();
}



class C{
	std::string s;
	public:
		C(std::string& s) : s{s}{
		}

		const std::string& toString() const{
			return this->s;
		}
};
namespace scl{
	namespace utils{
		template<>
		struct ToString<C, void>{
			std::string operator()(const C& c){
				return c.toString();
			}
		};
	}
}
TEST(ToStringTests, TemplateSpecializationWorks){
	std::string s = "yey";
	C c{s};
	ASSERT_EQ(s, asString(c));
}

struct X{
	int i = 0;
	X() = default;
	explicit X(int i) : i{i} {
	}
};
namespace std{
	string to_string(const X& x){
		return to_string(x.i);
	}
}

TEST(ToStringTests, StdToStringMakesItAvailable){
	static_assert(META::exists<ToString<char>>(), "toString<char> is not defined");
	static_assert(META::exists<ToString<short>>(), "toString<short> is not defined");
	static_assert(META::exists<ToString<int>>(), "toString<int> is not defined");
	static_assert(META::exists<ToString<size_t>>(), "toString<size_t> is not defined");
	static_assert(META::exists<ToString<long>>(), "toString<long> is not defined");
	static_assert(META::exists<ToString<long long>>(), "toString<long long> is not defined");
	static_assert(META::exists<ToString<float>>(), "toString<float> is not defined");
	static_assert(META::exists<ToString<double>>(), "toString<double> is not defined");
	static_assert(META::exists<ToString<long double>>(), "toString<long double> is not defined");

	static_assert(META::exists<ToString<X>>(), "toString<long double> is not defined");
	SUCCEED();
}

