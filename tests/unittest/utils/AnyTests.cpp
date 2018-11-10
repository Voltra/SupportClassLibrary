#include <gtest/gtest.h>
#include <scl/utils/Any.h>
#include <scl/concepts/concepts.hpp>

using namespace scl::concepts;
using namespace scl::utils;

TEST(AnyTests, concepts_met){
	ASSERT_TRUE(Movable<Any>{});
	ASSERT_TRUE(NonCopyable<Any>{});
}

