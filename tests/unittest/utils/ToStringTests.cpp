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
	ASSERT_TRUE(true);
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
	ASSERT_EQ(s, scl::asString(c));
}

