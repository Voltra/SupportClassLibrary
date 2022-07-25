#ifdef SCL_CPP17
#define SCL_MAYBE_UNUSED_ATTR [[maybe_unused]]
#else
#define SCL_MAYBE_UNUSED_ATTR
#endif

#define SCL_RETURN(EXPR) decltype((EXPR)) { return (EXPR); }