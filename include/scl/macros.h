#pragma once

#define realConst(type) const type const
#define stringLiteral const char*
#define assert_concept(cpt, str) static_assert(cpt, str)
#define static_require(cpt) assert_concept(cpt, "")

#define META scl::tools::meta
#define asString utils::toString