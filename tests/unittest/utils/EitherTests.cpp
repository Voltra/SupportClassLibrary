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

Either<int, const char*> makeLeft(){
	return Either<int, const char*>::Left(42);
}

Either<int, const char*> makeRight(){
	return Either<int, const char*>::Right("str(42)");
}

TEST(EitherTests, ConceptsRequirementsMet){
	auto either = makeLeft();
	auto e = either;
	using et = decltype(either);

	ASSERT_TRUE(
		Bool{
			META::is_trivially_movable<et::left_type>()
			&& META::is_trivially_movable<et::right_type>()
		}.implies(META::is_movable<et>())
	);

	ASSERT_TRUE(
		Bool{
			META::is_trivially_copyable<et::left_type>()
			&& META::is_trivially_copyable<et::right_type>()
		}.implies(META::is_copyable<et>())
	);
}