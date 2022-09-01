#pragma once
#include "../alias/byte.h"

namespace scl {
    namespace utils {
        template <class T, std::size_t Alignment = alignof(T)>
        using AlignedStorage = scl::alias::byte[sizeof(T)];
    }
}