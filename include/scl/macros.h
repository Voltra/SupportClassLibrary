#pragma once

#if  __cplusplus >= 201402L
#define SCL_CPP14
#endif

#if  __cplusplus >= 201703L
#define SCL_CPP17
#endif


#ifdef SCL_CPP17
#define SCL_MAYBE_UNUSED [[maybe_unused]]
#define SCL_NODISCARD [[nodiscard]]
#else
#define SCL_MAYBE_UNUSED
#define SCL_NODISCARD
#endif

#define SCL_RETURN(EXPR) decltype((EXPR)) { return (EXPR); }