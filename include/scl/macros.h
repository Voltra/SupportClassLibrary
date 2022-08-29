#pragma once

#if __cplusplus >= 201402L
    #define SCL_CPP14
#endif

#if __cplusplus >= 201703L
    #define SCL_CPP17
#endif

#ifdef SCL_CPP14
    #define SCL_CONSTEXPR14 constexpr
#else
    #define SCL_CONSTEXPR14
#endif

#ifdef SCL_CPP17
    #define SCL_MAYBE_UNUSED [[maybe_unused]]
    #define SCL_NODISCARD [[nodiscard]]
    #define SCL_CONSTEXPR17 constexpr
    #define SCL_INPLACE_T std::in_place_t
    #define SCL_INPLACE std::in_place
#else
    #define SCL_MAYBE_UNUSED
    #define SCL_NODISCARD
    #define SCL_CONSTEXPR17

struct scl_inplace_t {};
inline constexpr scl_inplace_t scl_inplace;

    #define SCL_INPLACE_T scl_inplace_t
    #define SCL_INPLACE scl_inplace
#endif

#define SCL_RETURN(expr...) \
    decltype(expr) { return expr; }