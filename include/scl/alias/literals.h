#pragma once
#include <chrono>
#include <complex>
#include <string>

namespace scl{
    namespace alias{
#ifdef SCL_CPP14
        using std::literals::complex_literals;
        using std::literals::chrono_literals;
        using std::literals::string_literals;
#endif

#ifdef SCL_CPP17
        using std::literals::string_view_literals;
#endif
    }
}