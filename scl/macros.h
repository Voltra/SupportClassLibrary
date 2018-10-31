#pragma once

#define realConst(type) const type const
#define stringLiteral const char*
#define assert_concept(cpt, str) static_assert(cpt, str)
#define META scl::tools::meta