#include <gtest/gtest.h>
#include <scl/utils/Any.h>
#include <scl/concepts/concepts.hpp>
#include <scl/exceptions/InvalidAnyCast.h>
#include <utility>
#include <string>

using namespace scl::concepts;
using namespace scl::utils;
using namespace scl::exceptions;

TEST(AnyTests, ConceptsRequirementsMet){
	ASSERT_TRUE(Movable<Any>{});
	ASSERT_TRUE(NonCopyable<Any>{});
}


TEST(AnyTests, CastingToUnrelatedTypeThrows) {
	Any any = static_cast<int>(42);
	ASSERT_THROW(any.as<std::string>(), InvalidAnyCast);
}

TEST(AnyTests, CannotCastToUnrelatedType){
	Any any = static_cast<int>(42);
	ASSERT_FALSE(any.canCastTo<std::string>());
}


TEST(AnyTests, CastingToRelatedTypeThrows) {
	Any any = static_cast<int>(42);
	ASSERT_THROW(any.as<float>(), InvalidAnyCast);
}

TEST(AnyTests, CannotCastToRelatedType){
	Any any = static_cast<int>(42);
	ASSERT_FALSE(any.canCastTo<float>());
}

TEST(AnyTests, CanCastToStoredType){
	Any any = static_cast<int>(42);
	ASSERT_TRUE(any.canCastTo<int>());
}

TEST(AnyTests, CastingToStoredTypeGivesCorrectValue){
	int value = 42;
	Any any{value};
	ASSERT_EQ(any.as<int>(), value);
}
