#include <gtest/gtest.h>
#include <scl/utils/Optional.h>
#include <scl/concepts/concepts.hpp>
#include <scl/exceptions/EmptyOptionalAccess.h>
#include <utility>
#include <string>
#include <iostream>

using namespace scl::concepts;
using namespace scl::exceptions;
using namespace scl::utils;

TEST(OptionalTests, ConceptsRequirementsMet){
	ASSERT_TRUE(DefaultConstructible<Optional<int>>{});
	ASSERT_TRUE(Movable<Optional<int>>{});
	ASSERT_TRUE(Copyable<Optional<int>>{});
}

TEST(OptionalTests, CanConstructFromNone){
	Optional<int> o{none};
}

TEST(OptionalTests, CanAssignFromNone){
	Optional<int> o = none;
}

TEST(OptionalTests, EmptyDoesNotHaveValue){
	Optional<int> o = none;
	ASSERT_FALSE(o.hasValue());
}

TEST(OptionalTests, NonEmptyHasValue){
	Optional<int> o = 42;
	ASSERT_TRUE(o.hasValue());
}

TEST(OptionalTests, GetValueOnEmptyThrows){
	Optional<int> o = none;
	ASSERT_THROW(o.get(), EmptyOptionalAccess);
}

TEST(OptionalTests, GetValueRetrievesCorrectValue){
	int value = 42;
	Optional<int> o = value;
	ASSERT_EQ(o.get(), value);
}

TEST(OptionalTests, MappingEmptyGivesEmpty){
	Optional<int> o = none;
	auto of = o.mapTo<float>([](const int& i){ return static_cast<float>(i); });
	ASSERT_EQ(of, none);
}

TEST(OptionalTests, FilteringFalseGivesEmpty){
	Optional<int> o = 42;
	auto oi = o.filter([](const int& i){ return false; });
	ASSERT_EQ(oi, none);
}

TEST(OptionalTests, FilteringTrueKeepsValue){
	int value = 42;
	Optional<int> o = value;
	auto oi = o.filter([](const int& i){ return true; });
	ASSERT_EQ(oi.value(), value);
}

TEST(OptionalTests, CanCompareWithConvertibleValueType){
	float gt = 43.f, lt = 41.f;
	int value = 42;
	float eq = static_cast<float>(value);
	Optional<int> o = value;

	ASSERT_FALSE(gt < o);
	std::cout << "\nlt: " << lt << ", o: " << o.value() << '\n';
	ASSERT_FALSE(lt > o);
	ASSERT_TRUE(eq == o);
	ASSERT_FALSE(lt >= o);
	ASSERT_FALSE(gt <= o);
	ASSERT_FALSE(eq != o);
}

TEST(OptionalTests, CanCompareWithOptionalOfConvertibleValueType){
	int value = 42;
	float fvalue = static_cast<float>(value);
	Optional<int> o = value;
	Optional<float> eq = fvalue, gt = fvalue + 1, lt = fvalue - 1;

	ASSERT_TRUE(eq == o);
	ASSERT_FALSE(eq != o);
	ASSERT_FALSE(gt < o);
	ASSERT_FALSE(gt <= o);
	ASSERT_FALSE(lt > o);
	ASSERT_FALSE(lt >= o);
}