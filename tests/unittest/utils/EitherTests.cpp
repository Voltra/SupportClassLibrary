#include <gtest/gtest.h>
#include <scl/utils/Either.h>
#include <testutils/Bool.h>
#include <scl/concepts/concepts.hpp>
#include <scl/tools/meta/type_check.h>
#include <scl/exceptions/InvalidEitherAccess.h>
#include <scl/tools/iostream/nl.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace scl::concepts;
using namespace scl::utils;
using namespace scl::exceptions;
using scl::tools::iostream::nl;

//TODO: Rendre Either compatible w/ type non trivialement copiables

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
}

TEST(EitherTests, ConstructingLeftMakesLeftAvailableAndRightNonAvailable){
	auto either = makeLeft();
	ASSERT_TRUE(either.hasLeft());
	ASSERT_FALSE(either.hasRight());
}