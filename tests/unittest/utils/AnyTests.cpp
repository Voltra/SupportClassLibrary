#include <gtest/gtest.h>
#include <scl/utils/Any.h>
#include <scl/concepts/concepts.hpp>
#include <scl/exceptions/InvalidAnyCast.h>
#include <utility>
#include <string>

using namespace scl::concepts;
using namespace scl::utils;
using namespace scl::exceptions;

TEST(AnyTests, concepts_met){
	ASSERT_TRUE(Movable<Any>{});
	ASSERT_TRUE(NonCopyable<Any>{});
}


TEST(AnyTests, casting_to_unrelated_type_throws) {
	Any any = static_cast<int>(42);
	ASSERT_THROW(any.as<std::string>(), InvalidAnyCast);
}

TEST(AnyTests, cannot_cast_to_unrelated_type){
	Any any = static_cast<int>(42);
	ASSERT_FALSE(any.canCastTo<std::string>());
}


TEST(AnyTests, casting_to_related_type_throws) {
	Any any = static_cast<int>(42);
	ASSERT_THROW(any.as<float>(), InvalidAnyCast);
}

TEST(AnyTests, cannot_cast_to_related_type){
	Any any = static_cast<int>(42);
	ASSERT_FALSE(any.canCastTo<float>());
}

TEST(AnyTests, can_cast_to_stored_type){
	Any any = static_cast<int>(42);
	ASSERT_TRUE(any.canCastTo<int>());
}

TEST(AnyTests, casting_to_stored_type_gives_correct_value){
	int value = 42;
	Any any{value};
	ASSERT_EQ(any.as<int>(), value);
}
