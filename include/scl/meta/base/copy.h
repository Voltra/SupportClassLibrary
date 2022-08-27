#pragma once
#include "../type_aliases/type_aliases.hpp"
#include "../type_queries/type_queries.hpp"

namespace scl {
    namespace meta {
        struct copyable_base {};

        struct non_copyable_base {
            non_copyable_base() = default;
            non_copyable_base(const non_copyable_base&) = delete;
            non_copyable_base& operator=(const non_copyable_base&) = delete;
        };

        template <class T>
        using copy_base = scl::meta::conditional_t<
            scl::meta::is_copyable<T>(),
            copyable_base,
            non_copyable_base
        >;
    }
}