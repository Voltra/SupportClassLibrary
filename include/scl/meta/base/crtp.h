#pragma once

namespace scl {
    namespace meta {
	template <class Derived>
	struct crtp_base {
		inline constexpr Derived* selfPtr_() noexcept {
		    return static_cast<Derived*>(this);
		}

		inline constexpr const Derived* selfPtr_() const noexcept {
		    return static_cast<const Derived*>(this);
		}

		inline constexpr Derived& self_() & noexcept { return *selfPtr_(); }

		inline constexpr Derived&& self_() && noexcept { return *selfPtr_(); }

		inline constexpr const Derived& self_() const& noexcept { return *selfPtr_(); }
	};
    }  // namespace meta
}  // namespace scl