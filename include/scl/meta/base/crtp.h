#pragma once

namespace scl {
    namespace meta {
        template <class Derived>
        struct crtp_base {
            constexpr inline Derived* selfPtr_() noexcept { return static_cast<Derived*>(this); }

            constexpr inline const Derived* selfPtr_() const noexcept {
                return static_cast<const Derived*>(this);
            }

            constexpr inline Derived& self_() noexcept { return *selfPtr_(); }

            constexpr inline const Derived& self_() const noexcept { return *selfPtr_(); }
        };
    }  // namespace meta
}  // namespace scl