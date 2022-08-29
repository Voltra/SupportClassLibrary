#pragma once
#include "../meta/meta.hpp"
#include "./RawStorage.h"

namespace scl {
    namespace utils {
        namespace details {
            struct either_left_tag {};
            struct either_right_tag {};

            constexpr static either_left_tag either_left{};
            constexpr static either_right_tag either_right{};

            struct TriviallyDestructibleEitherPayload {
                void destructor() noexcept {}

                ~TriviallyDestructibleEitherPayload() noexcept = default;
            };

            template <class L, class R, class Derived>
            struct DestructibleEitherPayload : public scl::meta::crtp_base<Derived> {
                void destructor() noexcept(scl::meta::is_nothrow_destructible<L>()
                                           && scl::meta::is_nothrow_destructible<R>()) {
                    auto self = this->self_();

                    if (self.valuelessByDestructor) {
                        return;
                    }

                    if (self.leftIsInit) {
                        self.leftPtr()->~L();
                    } else {
                        self.rightPtr()->~R();
                    }

                    self.valuelessByDestructor = true;
                }

                ~DestructibleEitherPayload() noexcept(scl::meta::is_nothrow_destructible<L>()
                                                      && scl::meta::is_nothrow_destructible<R>()) {
                    this->destructor();
                }
            };

            template <class L, class R, class Derived>
            using either_payload_destructor_impl
                = scl::meta::conditional_t<scl::meta::is_trivially_destructible<L>()
                                               && scl::meta::is_trivially_destructible<R>(),
                                           TriviallyDestructibleEitherPayload,
                                           DestructibleEitherPayload<L, R, Derived>>;

            template <class Left, class Right>
            class EitherPayload
                : public either_payload_destructor_impl<Left, Right, EitherPayload<Left, Right>> {
            public:
                using largest_value_type = scl::meta::largest_type_t<Left, Right>;
                using storage_type = scl::meta::aligned_storage_t<sizeof(largest_value_type),
                                                                  alignof(largest_value_type)>;

                using either_payload_destructor_impl<
                    Left, Right, EitherPayload<Left, Right>>::either_payload_destructible_impl;

            protected:
                storage_type storage;
                bool leftIsInit;
                bool valuelessByDestructor = false;

                template <class... Args>
                explicit EitherPayload(either_left_tag, Args&&... args)
                    : storage{}, leftIsInit{true} {
                    this->constructLeft(std::forward<Args>(args)...);
                }

                template <class... Args>
                explicit EitherPayload(either_right_tag, Args&&... args)
                    : storage{}, leftIsInit{false} {
                    new (this->rawPtr()) Right{std::forward<Args>(args)...};
                }

                template <class... Args>
                void constructLeft(Args&&... args) {
                    new (this->rawPtr()) Left{std::forward<Args>(args)...};
                    this->valuelessByDestructor = false;
                }

                template <class... Args>
                void constructRight(Args&&... args) {
                    new (this->rawPtr()) Right{std::forward<Args>(args)...};
                    this->valuelessByDestructor = false;
                }

                storage_type* rawPtr() { return &storage; }

                Left* leftPtr() { return reinterpret_cast<Left*>(rawPtr()); }

                Right* rightPtr() { return reinterpret_cast<Right*>(rawPtr()); }
            };
        }  // namespace details

        template <class LeftType, class RightType>
        class Either : private details::EitherPayload<LeftType, RightType> {
        public:
            using left_type = scl::meta::remove_cv_ref_t<LeftType>;
            using right_type = scl::meta::remove_cv_ref_t<RightType>;
            using either_type = Either;
            using payload_type = details::EitherPayload<left_type, right_type>;

            constexpr static bool left_nothrow_movable = scl::meta::is_nothrow_movable<left_type>();
            constexpr static bool right_nothrow_movable
                = scl::meta::is_nothrow_movable<left_type>();
            constexpr static bool nothrow_movable = left_nothrow_movable && right_nothrow_movable;

            using payload_type::EitherPayload;

        /*protected:
            payload_type payload;

            template <class... Args>
            explicit Either(details::either_left_tag tag, Args&&... args)
                : payload{tag, std::forward<Args>(args)...} {}

            template <class... Args>
            explicit Either(details::either_right_tag tag, Args&&... args)
                : payload{tag, std::forward<Args>(args)...} {}*/

        public:
            template <class... Args>
            static constexpr Either Left(Args&&... args) {
                return Either{details::either_left, std::forward<Args>(args)...};
            }

            /**
             * Alias for scl::utils::Either::Left
             * @tparam Args
             * @param args
             * @return
             */
            template <class... Args>
            static constexpr Either left(Args&&... args) {
                return Left(std::forward<Args>(args)...);
            }

            template <class... Args>
            static constexpr Either Right(Args&&... args) {
                return Either{details::either_right, std::forward<Args>(args)...};
            }

            /**
             * Alias for scl::utils::Either::Right
             * @tparam Args
             * @param args
             * @return
             */
            template <class... Args>
            static constexpr Either right(Args&&... args) {
                return Right(std::forward<Args>(args)...);
            }

            Either() = delete;

            Either(Either&& other) noexcept(Either::nothrow_movable) = default;
            Either& operator=(Either&& rhs) noexcept(Either::nothrow_movable) = default;

            constexpr bool isLeft() const { return this->leftIsInit; }

            constexpr bool isRight() const { return !this->isLeft(); }
        };
    }  // namespace utils
}  // namespace scl