#pragma once
#include "../meta/meta.hpp"

namespace scl {
    namespace utils {
        template <class LeftType, class RightType>
        class Either {
        public:
            using left_type = scl::meta::remove_cv_ref_t<LeftType>;
            using right_type = scl::meta::remove_cv_ref_t<RightType>;

            constexpr static bool left_nothrow_movable = scl::meta::is_nothrow_movable<left_type>();
            constexpr static bool right_nothrow_movable = scl::meta::is_nothrow_movable<left_type>();
            constexpr static bool nothrow_movable = left_nothrow_movable && right_nothrow_movable;

        protected:
            struct left_tag{};
            struct right_tag{};

            Either(left_tag, left_type leftValue) {}
            Either(right_tag, right_type rightValue) {}

        public:
            Either() = delete;

            Either(Either&& other) noexcept(Either::nothrow_movable) = default;
            Either& operator=(Either&& rhs) noexcept(Either::nothrow_movable) = default;
        };
    }
}