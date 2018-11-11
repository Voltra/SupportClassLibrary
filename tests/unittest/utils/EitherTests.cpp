#include <gtest/gtest.h>
#include <scl/utils/Either.h>
#include <scl/concepts/concepts.hpp>
#include <scl/tools/meta/type_check.h>
#include <scl/exceptions/InvalidEitherAccess.h>

using namespace scl::concepts;
using namespace scl::utils;
using namespace scl::exceptions;

Either<int, std::string> makeLeft(){
	return Either<int, std::string>::Left(42);
}

Either<int, std::string> makeRight(){
	return Either<int, std::string>::Right("str(42)");
}

class Bool{
	protected:
		bool value;

	public:
		Bool() = delete;
		explicit Bool(bool b) : value{b} {
		};
		Bool(const Bool&) = default;
		Bool(Bool&&) = default;

		Bool& operator=(const Bool&) = default;
		Bool& operator=(Bool&&) noexcept = default;

		operator bool() const{ return value; }

		template <class B>
		Bool implies(B b) const{ return Bool{!(*this) || b}; }

		template <class B>
		Bool equiv(B b) const{ return Bool{this->implies(b) && Bool{b}.implies(*this)}; }
};

TEST(EitherTests, ConceptsRequirementsMet){
	auto either = makeLeft();

	ASSERT_TRUE(
		Bool{
			META::is_move_assignable<decltype(either)::left_type>()
			&& META::is_move_assignable<decltype(either)::right_type>()
		}.implies(META::is_move_assignable<decltype(either)>())
	);

	ASSERT_TRUE(
		Bool{
			META::is_move_constructible<decltype(either)::left_type>()
			&& META::is_move_constructible<decltype(either)::right_type>()
		}.implies(META::is_move_constructible<decltype(either)>())
	);

	ASSERT_TRUE(
		Bool{
			META::is_copy_assignable<decltype(either)::left_type>()
			&& META::is_copy_assignable<decltype(either)::right_type>()
		}.implies(META::is_copy_assignable<decltype(either)>())
	);

	ASSERT_TRUE(
		Bool{
			META::is_copy_constructible<decltype(either)::left_type>()
			&& META::is_copy_constructible<decltype(either)::right_type>()
		}.implies(META::is_copy_constructible<decltype(either)>())
	);
}