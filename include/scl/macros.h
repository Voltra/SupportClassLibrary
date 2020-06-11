#pragma once

#define realConst(type) const type const
#define stringLiteral const char*
#define assert_concept(cpt, str) static_assert(cpt, str)
#define static_require(cpt) assert_concept(cpt, "")

#define META scl::tools::meta
#define asString scl::utils::toString
#define SCL_RETURNS(stmt) -> decltype(stmt){\
	return stmt;\
}

#ifdef SCL_CPP20
	#define SCL_CPP17
#endif

#ifdef SCL_CPP17
	#define SCL_CPP14
#endif