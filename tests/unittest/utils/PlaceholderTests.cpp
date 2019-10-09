#include <gtest/gtest.h>
#include <scl/utils/Placeholder.h>
#include <utility>
#include <functional>
#include <string>
#include <scl/tools/make/optional.h>

using namespace scl::utils;
namespace make = scl::tools::make;

struct ptr_cp{
	mutable int copyCount = 0;

	ptr_cp(){}
	ptr_cp(const ptr_cp& other) {
		other.copyCount += 1;
	}
	ptr_cp(ptr_cp&&) = delete;
	ptr_cp& operator=(ptr_cp&&) = delete;
};

TEST(PlaceholderTests, CanMoveAndIsNoOp){
	auto value = "42";
	auto opt = make::some<std::string>(value);
	placeholder::_ = std::move(opt);
	ASSERT_TRUE(opt.hasValue());
	ASSERT_EQ(*opt, value);
}

TEST(PlaceholderTests, CanMoveConstructAndIsNoOp){
	auto value = "42";
	auto opt = make::some<std::string>(value);
	Placeholder p{std::move(opt)};

	ASSERT_TRUE(opt.hasValue());
	ASSERT_EQ(*opt, value);
}

TEST(PlaceholderTests, CanCopyAndIsNoOp){
	ptr_cp ptr;
	placeholder::_ = ptr;
	ASSERT_EQ(ptr.copyCount, 0);
}

TEST(PlaceholderTests, CanCopyConstructAndIsNoOp){
	ptr_cp ptr;
	Placeholder p{ptr};
	ASSERT_EQ(ptr.copyCount, 0);
}

TEST(PlaceholderTests, CanUseInTie){
	int x = -1, val = 2;
	std::tie(placeholder::_, x, placeholder::_) = std::make_tuple(val/2, val, val*2);

	ASSERT_TRUE(x >= 0);
	ASSERT_EQ(x, val);
}

#ifdef SCL_CPP17
	TEST(PlaceholderTests, CanUseInStructuredBinding){
		int val = 10;
		auto [placeholder::_, x, placeholder::_] = std::make_tuple(val / 2, val, val * 2);
		ASSERT_EQ(x, val);
	}
#endif