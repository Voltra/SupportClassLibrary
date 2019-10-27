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

using leftType = int;
using rightType = std::string;
using eitherType = Either<leftType, rightType>;

eitherType makeLeft(int i){
	return eitherType::Left(i);
}

eitherType makeRight(std::string s){
	return eitherType::Right(std::move(s));
}

TEST(EitherTests, ConceptsRequirementsMet){
	auto either = makeLeft(42);
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

	/*ASSERT_TRUE(
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
	);*/
}

TEST(EitherTests, ConstructingLeftMakesLeftAvailableAndRightNonAvailable){
	auto either = makeLeft(42);
	ASSERT_TRUE(either.hasLeft());
	ASSERT_FALSE(either.hasRight());
}
TEST(EitherTests, ConstructingRightMakesRightAvailableAndLeftNonAvailable){
	auto either = makeRight("str(42)");
	ASSERT_TRUE(either.hasRight());
	ASSERT_FALSE(either.hasLeft());
}



TEST(EitherTests, RetrievingLeftOnLeftGivesTheSameValue){
	auto value = 42;
	auto either = makeLeft(value);
	ASSERT_EQ(value, either.getLeft());
}
TEST(EitherTests, RetrievingLeftOnRightGivesTheSameValue){
	auto value = "str(42)";
	auto either = makeRight(value);
	ASSERT_EQ(value, either.getRight());
}

TEST(EitherTests, RetrievingWrongSideThrows){
	ASSERT_THROW(makeLeft(42).getRight(), InvalidEitherAccess);
	ASSERT_THROW(makeRight("str").getLeft(), InvalidEitherAccess);
}

TEST(EitherTests, VisitCallsTheCorrectBranch){
	auto lefty = makeLeft(42);
	auto righty = makeRight("str(42)");

	lefty.visit(
		[](const leftType&){ SUCCEED(); },
		[](const rightType&){ FAIL(); }
	);

	righty.visit(
		[](const leftType&){ FAIL(); },
		[](const rightType&){ SUCCEED(); }
	);
}

TEST(EitherTests, DoIfLeftCallsOnlyOnLeft){
	auto lefty = makeLeft(42);
	auto righty = makeRight("str(42)");

	lefty.doIfLeft([](const leftType&){ SUCCEED(); });
	righty.doIfLeft([](const leftType&){ FAIL(); });
}
TEST(EitherTests, DoIfRightCallsOnlyOnRight){
	auto lefty = makeLeft(42);
	auto righty = makeRight("str(42)");

	righty.doIfRight([](const rightType&){ SUCCEED(); });
	lefty.doIfRight([](const rightType&){ FAIL(); });
}

TEST(EitherTests, MappingChangesOnlyTheActiveAlternative){
	leftType lvalue = 42;
	rightType rvalue = "str(42)";
	auto lefty = makeLeft(lvalue);
	auto righty = makeRight(rvalue);

	auto incr = [](const leftType& old){ return old+1; };
	auto void_ = [](const rightType&){ return ""; };

	auto nlefty = lefty.mapLeftTo<leftType>(incr);
	auto nolefty = righty.mapLeftTo<leftType>(incr);
	ASSERT_FALSE(nlefty.hasRight()); ASSERT_FALSE(nolefty.hasLeft());
	ASSERT_TRUE(nlefty.hasLeft()); ASSERT_TRUE(nolefty.hasRight());
	ASSERT_EQ(nlefty.getLeft(), lvalue + 1);
	ASSERT_EQ(nolefty.getRight(), rvalue); //TODO: Check string corruption


	auto nrighty = righty.mapRightTo<rightType>(void_);
	auto norighty = lefty.mapRightTo<rightType>(void_);
	ASSERT_TRUE(nrighty.hasRight()); ASSERT_TRUE(norighty.hasLeft());
	ASSERT_FALSE(nrighty.hasLeft()); ASSERT_FALSE(norighty.hasRight());
	ASSERT_EQ(nrighty.getRight(), void_(""));
	ASSERT_EQ(norighty.getLeft(), lvalue);
}

TEST(EitherTests, LeftOrOnRightGivesDefault){
	auto either = makeRight("strtr");
	leftType default_ = 28;

	ASSERT_EQ(either.leftOr(default_), default_);
}
TEST(EitherTests, LeftOrOnLeftGivesValue){
	leftType value = 42;
	leftType default_ = value + 4;
	auto either = makeLeft(value);

	ASSERT_EQ(either.leftOr(default_), value);
}

TEST(EitherTests, RightOrOnLeftGivesDefault){
	auto either = makeLeft(42);
	rightType default_ = "str(42)";

	ASSERT_EQ(either.rightOr(default_), default_);
}
TEST(EitherTests, RightOrOnRightGivesValue){
	rightType value = "42";
	rightType default_ = value + "4";
	auto either = makeRight(value);

	ASSERT_EQ(either.rightOr(default_), value);
}

//TODO: Either::map
//TODO: Either::leftOr
//TODO: Either::rightOr