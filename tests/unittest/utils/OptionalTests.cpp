#include <gtest/gtest.h>
#include <scl/utils/Optional.h>
#include <testutils/Bool.h>
#include <testutils/AdvancedTypes.h>

#include <scl/concepts/concepts.hpp>
#include <scl/exceptions/EmptyOptionalAccess.h>
#include <scl/tools/make/make.hpp>
#include <scl/tools/iostream/nl.h>

#include <utility>
#include <string>
#include <iostream>

using namespace scl::concepts;
using namespace scl::exceptions;
using namespace scl::utils;
using namespace scl::tools;
using scl::tools::iostream::nl;

TEST(OptionalTests, ConceptsRequirementsMet) {
	/*{
		Optional<int> o = 42;
		Optional<int> x{o};
		x = o;
		std::cout << "x: " << x.get() << ", o: " << o.get() << nl;
	}*/

	ASSERT_TRUE(DefaultConstructible<Optional<int>>{});
	ASSERT_TRUE(Movable<Optional<int>>{});
	ASSERT_TRUE(Copyable<Optional<int>>{});

	ASSERT_TRUE(Bool{META::is_copyable<int>()}.equiv(META::is_copyable<Optional<int>>()));
	ASSERT_TRUE(Bool{META::is_movable<int>()}.equiv(META::is_movable<Optional<int>>()));

	ASSERT_TRUE(Bool{META::is_copyable<std::string>()}.equiv(META::is_copyable<Optional<std::string>>()));
	ASSERT_TRUE(Bool{META::is_movable<std::string>()}.equiv(META::is_movable<Optional<std::string>>()));
}

TEST(OptionalTests, CanConstructFromNone){
	Optional<int> o{none};
}

TEST(OptionalTests, CanAssignFromNone){
	Optional<int> o = none;
}

TEST(OptionalTests, CanUseNonTriviallyCopyableType){
	Optional<std::string> o = "42";
	auto p = o;
	ASSERT_EQ(o.get(), p.get());

	auto op = make::ptr<Optional<std::string>>("42");
	auto op2 = *op;
	delete op;
	ASSERT_EQ(op2.get(), "42");
}

TEST(OptionalTests, CanUseNonTriviallyMovableType){
	const std::string value = "42";
	Optional<std::string> o = value;
	Optional<std::string> p = std::move(o);
	ASSERT_EQ(p.get(), value);
}

TEST(OptionalTests, MovedFromHasCorrectSemantics){
	Optional<std::string> no = none;
	auto a = std::move(no);

	ASSERT_EQ(a.hasValue(), no.hasValue());
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
	ASSERT_TRUE(of == none);
}

TEST(OptionalTests, FilteringFalseGivesEmpty){
	Optional<int> o = 42;
	auto oi = o.filter([](const int& i){ return false; });
	ASSERT_TRUE(oi == none);
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

TEST(OptionalTests, ComparingWithNoneMakesSureItIsLessThanOrEqual){
	Optional<int> notEmpty = 42;
	Optional<int> empty = none;

	ASSERT_TRUE(none < notEmpty);
	ASSERT_TRUE(none <= notEmpty);
	ASSERT_FALSE(none > notEmpty);
	ASSERT_FALSE(none >= notEmpty);
	ASSERT_FALSE(none == notEmpty);

	ASSERT_TRUE(none == empty);
	ASSERT_TRUE(none <= empty);
	ASSERT_TRUE(none >= empty);
	ASSERT_FALSE(none != empty);
}

TEST(OptionalTests, ComparingNoneWithNoneMakesSense){
	None nil{};

	ASSERT_TRUE(none == nil);
	ASSERT_FALSE(none != nil);
	ASSERT_FALSE(none < nil);
	ASSERT_FALSE(none > nil);
	ASSERT_TRUE(none >= nil);
	ASSERT_TRUE(none <= nil);
}

TEST(OptionalTests, GetBackDefaultWhenEmpty){
	Optional<int> o = none;
	int defaultValue = 42;
	ASSERT_EQ(o.orElse(defaultValue), defaultValue);
}

TEST(OptionalTests, ThrowsGivenExceptionOnEmpty){
	Optional<int> o = none;
	Exception e{"Exception/20"};
	ASSERT_THROW(o.orThrow(e), decltype(e));
}

TEST(OptionalTests, AdvancedInvalidTypesHaveWellDefinedBehavior){
	/*using nm = Optional<NonMovable_t>;
	using nc = Optional<NonCopyable_t>;*/
	using nmnc = Optional<NonCopyableNonMovable_t>;

	ASSERT_EQ(META::is_copyable<NonCopyableNonMovable_t>(), META::is_copyable<nmnc>());
	ASSERT_EQ(META::is_movable<NonCopyableNonMovable_t>(), META::is_movable<nmnc>());
}