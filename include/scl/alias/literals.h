#pragma once
#ifdef SCL_CPP14
    #include <chrono>
    #include <complex>
    #include <string>
#endif

#ifdef SCL_CPP17
    #include <string_view>
#endif

namespace scl {
    namespace alias {
#ifdef SCL_CPP14
        using namespace std::literals::chrono_literals;
        using namespace std::literals::complex_literals;
        using namespace std::literals::string_literals;
#endif

#ifdef SCL_CPP17
        using namespace std::literals::string_view_literals;
#endif
    }  // namespace alias
}  // namespace scl