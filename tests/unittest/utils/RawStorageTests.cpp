#include <gtest/gtest.h>
#include <scl/utils/RawStorage.h>
#include <scl/macros.h>
#include <scl/concepts/concepts.hpp>
#include <mutex>
#include <scl/tools/iostream/log/log.hpp>
#include <scl/exceptions/UninitializedMemoryAccess.h>
#include <scl/utils/Placeholder.h>
#include <utility>

using namespace scl::utils;
using namespace scl::concepts;
using namespace scl::exceptions;
using namespace scl::utils::placeholder;

auto out = scl::tools::iostream::log::prefixed::log("[S]  ");

TEST(RawStorageTests, ConceptsRequirementsMet){
	using rs = RawStorage<std::mutex>;
	ASSERT_TRUE(DefaultConstructible<rs>{});
	ASSERT_TRUE(Movable<rs>{});
	ASSERT_TRUE(Destructible<rs>{});
}

struct FailOnInit{
	FailOnInit(){
		throw 42;
	}
};

TEST(RawStorageTests, CreatingStorageDoesNotCreateObject){
	using rs = RawStorage<FailOnInit>;
	ASSERT_NO_THROW(rs{});
	rs x{};
	ASSERT_FALSE(x.hasValue());
}

struct Ss{
	int i;

	Ss() : i{42} {
	}
};
TEST(RawStorageTests, ConstructCreatesObject){
	using rs = RawStorage<Ss>;
	rs x;
	ASSERT_FALSE(x.hasValue());

	x.construct();
	ASSERT_TRUE(x.hasValue());
	ASSERT_EQ(x.get().i, 42);
}

TEST(RawStorageTests, DestructorDestroysObject){
	using rs = RawStorage<Ss>;
	rs x;
	ASSERT_FALSE(x.hasValue());

	x.construct();
	ASSERT_TRUE(x.hasValue());

	x.destructor();
	ASSERT_FALSE(x.hasValue());
}

struct Si{
	static int i;
	Si(){
		++i;
	}
	~Si(){
		--i;
	}
};
int Si::i = 0;

TEST(RawStorageTests, RAII_WhenLeavesScope){
	using rs = RawStorage<Si>;

	ASSERT_EQ(Si::i, 0);
	{
		rs x;
		x.construct();
		ASSERT_EQ(Si::i, 1);
	}
	ASSERT_EQ(Si::i, 0);
}

TEST(RawStorageTests, ConstructingOverValueCallsDestructor){
	using rs = RawStorage<Si>;

	ASSERT_EQ(Si::i, 0);
	{
		rs x;
		x.construct();
		ASSERT_EQ(Si::i, 1);
		x.construct(); //--i then ++i
		ASSERT_EQ(Si::i, 1);
	}
	ASSERT_EQ(Si::i, 0);
}

TEST(RawStorageTests, AccessUninitializedThrows){
	using rs = RawStorage<std::string>;

	rs x;
	ASSERT_THROW(x.get(), UninitializedMemoryAccess);
	ASSERT_THROW(_ = x->begin(), UninitializedMemoryAccess);
	ASSERT_THROW(*x, UninitializedMemoryAccess);
}

TEST(RawStorageTests, IsConvertibleToBool){
	using rs = RawStorage<std::string>;

	rs x;
	ASSERT_FALSE(x);

	x.constructor();
	ASSERT_TRUE(x);

	x.destructor();
	ASSERT_FALSE(x);
}


TEST(RawStorageTests, CanRetrieveTheValue){
	using rs = RawStorage<std::string>;

	rs x;
	x.constructor();
	ASSERT_EQ(*x, "");

	auto value = "128";
	x.get() += value;
	ASSERT_EQ(*x, value);
}

TEST(RawStorageTests, MoveSemanticsAreGuaranteed){
	using rs = RawStorage<std::unique_ptr<std::mutex>>;

	rs x;
	ASSERT_FALSE(x.hasValue());

	x.constructor();
	ASSERT_TRUE(x.hasValue());

	rs y;
	ASSERT_FALSE(y.hasValue());

	y = std::move(x);
	ASSERT_TRUE(y.hasValue());//TODO: Fix memory corruption
	ASSERT_FALSE(x.hasValue()); //guarantees use-after-move semantics (safeguards)


	{ //Testing value conservation after move

		using RS = RawStorage<std::string>;
		RS x, y;
		auto value = "42";
		x.constructor(value);

		y = std::move(x);
		ASSERT_EQ(y.get(), value);
	}
}

