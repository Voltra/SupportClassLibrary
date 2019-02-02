#include <gtest/gtest.h>
#include <scl/utils/Either.h>
#include <testutils/Bool.h>
#include <testutils/AdvancedTypes.h>

#include <scl/concepts/concepts.hpp>
#include <scl/tools/meta/type_check.h>
#include <scl/exceptions/InvalidEitherAccess.h>
#include <scl/tools/iostream/nl.h>

#include <iostream>
#include <memory>

using namespace scl::concepts;
using namespace scl::utils;
using namespace scl::exceptions;
using scl::tools::iostream::nl;



Either<int, std::string> makeLeft(){
	return Either<int, std::string>::Left(42);
}

Either<int, std::string> makeRight(){
	return Either<int, std::string>::Right("str(42)");
}

TEST(EitherTests, ConceptsRequirementsMet){
	auto either = makeLeft();
	using et = decltype(either);

	ASSERT_TRUE(
		Bool{
			META::is_movable<et::left_type>()
			&& META::is_movable<et::right_type>()
		}.equiv(META::is_movable<et>())
	);

	ASSERT_TRUE(
		Bool{
			META::is_copyable<et::left_type>()
			&& META::is_copyable<et::right_type>()
		}.equiv(META::is_copyable<et>())
	);



	using nmnc = Either<NonMovable_t, NonCopyable_t>;

	ASSERT_TRUE(
		Bool{
			META::is_movable<nmnc::left_type>()
			&& META::is_movable<nmnc::right_type>()
		}.equiv(META::is_movable<nmnc>())
	);

	ASSERT_TRUE(
		Bool{
			META::is_copyable<nmnc::left_type>()
			&& META::is_copyable<nmnc::right_type>()
		}.equiv(META::is_copyable<nmnc>())
	);
}

TEST(EitherTests, ConstructingLeftMakesLeftAvailableAndRightNonAvailable){
	auto either = makeLeft();
	ASSERT_TRUE(either.hasLeft());
	ASSERT_FALSE(either.hasRight());
}

TEST(EitherTests, ConstructingRightMakesRightAvailableAndLeftNonAvailable){
	auto either = makeRight();
	ASSERT_TRUE(either.hasRight());
	ASSERT_FALSE(either.hasLeft());
}