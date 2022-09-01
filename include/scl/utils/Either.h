#pragma once
#include "../exceptions/InvalidEitherAccess.h"
#include "../macros.h"
#include "../meta/base/base.hpp"
#include "../meta/type_aliases/callables.h"
#include "../meta/type_aliases/largest_type.h"
#include "../meta/type_aliases/stl.h"
#include "../meta/type_queries/stl.h"
#include "./AlignedStorage.h"
#include "./RawStorage.h"
#include "./invoke.h"

#if SCL_NO_EXCEPTIONS
    #warning \
	"scl::utils::Either's getLeft() and getRight() are unsafe without exceptions. Make sure you check your object's lifetime (i.e. no use-after-move) and use isLeft() and isRight()"
#endif

namespace scl {
    namespace utils {
	namespace details {
	    struct either_left_tag {};
	    struct either_right_tag {};

	    static constexpr either_left_tag either_left{};
	    static constexpr either_right_tag either_right{};

	    struct TriviallyDestructibleEitherPayload {
		    void destructor() noexcept {}

		    ~TriviallyDestructibleEitherPayload() noexcept = default;
	    };

	    template <class L, class R, class Derived>
	    struct DestructibleEitherPayload : public scl::meta::crtp_base<Derived> {
		    void destructor() noexcept(scl::meta::is_nothrow_destructible<L>()
					       && scl::meta::is_nothrow_destructible<R>()) {
			auto& self = this->self_();

			if (self.valuelessByDestructor) {
			    return;
			}

			self.valuelessByDestructor = true;

			if (self.leftIsInit) {
			    self.leftPtr()->~L();
			} else {
			    self.rightPtr()->~R();
			}
		    }

		    ~DestructibleEitherPayload() noexcept(
			scl::meta::is_nothrow_destructible<L>()
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

	    template <class L, class R, class Derived>
	    struct CopyableEitherPayload {
		    CopyableEitherPayload() noexcept = default;
		    CopyableEitherPayload(const CopyableEitherPayload& other) noexcept(
			scl::meta::is_nothrow_copyable<L>()
			&& scl::meta::is_nothrow_copyable<R>()) {
			auto* lhs = static_cast<Derived*>(this);
			auto&& rhs = static_cast<const Derived&>(other);

			if (!rhs.valuelessByDestructor) {
			    if (rhs.leftIsInit) {
				lhs->constructLeft(*rhs.leftPtr());
			    } else {
				lhs->constructRight(*rhs.rightPtr());
			    }
			} else {
			    lhs->valuelessByDestructor = true;
			}
		    };

		    CopyableEitherPayload& operator=(const CopyableEitherPayload& other) noexcept(
			scl::meta::is_nothrow_copyable<L>()
			&& scl::meta::is_nothrow_copyable<R>()) {
			auto* lhs = static_cast<Derived*>(this);
			auto&& rhs = static_cast<const Derived&>(other);

			lhs->destructor();

			if (!rhs.valuelessByDestructor) {
			    if (rhs.leftIsInit) {
				lhs->constructLeft(*rhs.leftPtr());
			    } else {
				lhs->constructRight(*rhs.rightPtr());
			    }
			} else {
			    lhs->valuelessByDestructor = true;
			}

			return *this;
		    };
	    };

	    template <class L, class R, class Derived>
	    using either_payload_copy_impl = scl::meta::conditional_t<
		scl::meta::is_copyable<L>() && scl::meta::is_copyable<R>(),
		CopyableEitherPayload<L, R, Derived>, scl::meta::non_copyable_base>;

	    template <class Left, class Right>
	    class EitherPayload
		: public either_payload_destructor_impl<Left, Right, EitherPayload<Left, Right>>,
		  public either_payload_copy_impl<Left, Right, EitherPayload<Left, Right>> {
		public:
		    friend either_payload_destructor_impl<Left, Right, EitherPayload<Left, Right>>;
		    friend either_payload_copy_impl<Left, Right, EitherPayload<Left, Right>>;

		    using largest_value_type = scl::meta::largest_type_t<Left, Right>;
		    using storage_type = AlignedStorage<largest_value_type>;

		    using either_payload_destructor_impl<
			Left, Right, EitherPayload<Left, Right>>::either_payload_destructor_impl;

		    using either_payload_copy_impl<
			Left, Right, EitherPayload<Left, Right>>::either_payload_copy_impl;

		    using either_payload_copy_impl<Left, Right,
						   EitherPayload<Left, Right>>::operator=;

		protected:
		    alignas(largest_value_type) storage_type storage;
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

		    EitherPayload(EitherPayload&& other) noexcept(
			scl::meta::is_nothrow_movable<Left>()
			&& scl::meta::is_nothrow_movable<Right>())
			: storage{},
			  leftIsInit{other.leftIsInit},
			  valuelessByDestructor{other.valuelessByDestructor} {
			if (!other.valuelessByDestructor) {
			    if (leftIsInit) {
				this->constructLeft(std::move(*other.leftPtr()));
			    } else {
				this->constructRight(std::move(*other.rightPtr()));
			    }

			    other.destructor();
			}
		    }

		    EitherPayload& operator=(EitherPayload&& other) noexcept(
			scl::meta::is_nothrow_movable<Left>()
			&& scl::meta::is_nothrow_movable<Right>()) {
			this->destructor();

			if (!other.valuelessByDestructor) {
			    leftIsInit = other.leftIsInit;

			    if (leftIsInit) {
				this->constructLeft(std::move(*other.leftPtr()));
			    } else {
				this->constructRight(std::move(*other.rightPtr()));
			    }

			    other.destructor();
			}

			return *this;
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

		    constexpr void* rawPtr() noexcept {
			return storage;
		    }

		    constexpr Left* leftPtr() noexcept {
			return static_cast<Left*>(rawPtr());
		    }

		    constexpr Right* rightPtr() noexcept {
			return static_cast<Right*>(rawPtr());
		    }
	    };
	}  // namespace details

	template <class LeftType, class RightType>
	class Either : private details::EitherPayload<LeftType, RightType> {
	    public:
		using left_type = scl::meta::remove_cv_ref_t<LeftType>;
		using right_type = scl::meta::remove_cv_ref_t<RightType>;
		using either_type = Either;
		using payload_type = details::EitherPayload<left_type, right_type>;
		using storage_type = typename payload_type::storage_type;

		using details::EitherPayload<left_type, right_type>::EitherPayload;

		template <class Lhs, class Rhs>
		friend class Either;

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

		constexpr bool isLeft() const noexcept {
		    return !this->valuelessByDestructor && this->leftIsInit;
		}

		constexpr bool isRight() const noexcept {
		    return !this->valuelessByDestructor && !this->leftIsInit;
		}

		left_type& getLeft() & noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
		    if (!isLeft()) {
			throw scl::exceptions::InvalidEitherAccess::leftWhenRight();
		    }
#endif

		    return unsafeGetLeft();
		}

		const left_type& getLeft() const& noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
		    if (!isLeft()) {
			throw scl::exceptions::InvalidEitherAccess::leftWhenRight();
		    }
#endif

		    return unsafeGetLeft();
		}

		left_type&& getLeft() && noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
		    if (!isLeft()) {
			throw scl::exceptions::InvalidEitherAccess::leftWhenRight();
		    }
#endif

		    return std::move(unsafeGetLeft());
		}

		right_type& getRight() & noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
		    if (!isRight()) {
			throw scl::exceptions::InvalidEitherAccess::rightWhenLeft();
		    }
#endif

		    return unsafeGetRight();
		}

		const right_type& getRight() const& noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
		    if (!isRight()) {
			throw scl::exceptions::InvalidEitherAccess::rightWhenLeft();
		    }
#endif

		    return unsafeGetRight();
		}

		right_type&& getRight() && noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
		    if (!isRight()) {
			throw scl::exceptions::InvalidEitherAccess::leftWhenRight();
		    }
#endif

		    return std::move(unsafeGetRight());
		}

		template <class LeftVisitor, class RightVisitor>
		Either& visit(LeftVisitor&& visitLeft, RightVisitor&& visitRight) & {
		    if (this->isLeft()) {
			invoke(std::forward<LeftVisitor>(visitLeft), unsafeGetLeft());
		    } else if (this->isRight()) {
			invoke(std::forward<RightVisitor>(visitRight), unsafeGetRight());
		    }

		    return *this;
		}

		template <class LeftVisitor, class RightVisitor>
		const Either& visit(LeftVisitor&& visitLeft, RightVisitor&& visitRight) const& {
		    if (this->isLeft()) {
			invoke(std::forward<LeftVisitor>(visitLeft), unsafeGetLeft());
		    } else if (this->isRight()) {
			invoke(std::forward<RightVisitor>(visitRight), unsafeGetRight());
		    }

		    return *this;
		}

		template <class LeftVisitor, class RightVisitor>
		void visit(LeftVisitor&& visitLeft, RightVisitor&& visitRight) && {
		    if (this->isLeft()) {
			invoke(std::forward<LeftVisitor>(visitLeft), std::move(unsafeGetLeft()));
		    } else if (this->isRight()) {
			invoke(std::forward<RightVisitor>(visitRight), std::move(unsafeGetRight()));
		    }

		    /*
		     * At this point, any use would be a use-after-move.
		     * So the signature was changed to return void to avoid
		     * having to deal with that.
		     */
		}

		template <class LeftVisitor>
		Either& doIfLeft(LeftVisitor&& visitLeft) & {
		    if (this->isLeft()) {
			invoke(std::forward<LeftVisitor>(visitLeft), unsafeGetLeft());
		    }

		    return *this;
		}

		template <class LeftVisitor>
		const Either& doIfLeft(LeftVisitor&& visitLeft) & {
		    if (this->isLeft()) {
			invoke(std::forward<LeftVisitor>(visitLeft), unsafeGetLeft());
		    }

		    return *this;
		}

		template <class LeftVisitor>
		Either&& doIfLeft(LeftVisitor&& visitLeft) && {
		    if (this->isLeft()) {
			invoke(std::forward<LeftVisitor>(visitLeft), std::move(unsafeGetLeft()));
		    }

		    /*
		     * At this point, if isLeft() was true,
		     * any use of the left "branch" is a use-after-move
		     */

		    return std::move(*this);
		}

		template <class RightVisitor>
		Either& doIfRight(RightVisitor&& visitRight) & {
		    if (this->isRight()) {
			invoke(std::forward<RightVisitor>(visitRight), unsafeGetRight());
		    }

		    /*
		     * At this point, if isLeft() was true,
		     * any use of the right "branch" is a use-after-move
		     */

		    return *this;
		}

		template <class RightVisitor>
		const Either& doIfRight(RightVisitor&& visitRight) & {
		    if (this->isRight()) {
			invoke(std::forward<RightVisitor>(visitRight), unsafeGetRight());
		    }

		    return *this;
		}

		template <class RightVisitor>
		Either&& doIfRight(RightVisitor&& visitRight) && {
		    if (this->isRight()) {
			invoke(std::forward<RightVisitor>(visitRight), std::move(unsafeGetRight()));
		    }

		    return std::move(*this);
		}

		template <class Mapper, class Lhs = scl::meta::return_t<Mapper>>
		Either<Lhs, right_type> mapLeft(Mapper&& mapper) const& {
		    if (this->isRight()) {
			return Either<Lhs, right_type>::Right(this->unsafeGetRight());
		    } else if (this->isLeft()) {
			return Either<Lhs, right_type>::Left(
			    invoke(std::forward<Mapper>(mapper), this->unsafeGetLeft()));
		    } else {
			return Either<Lhs, right_type>::valuelessInstance();
		    }
		}

		template <class Mapper, class Lhs = scl::meta::return_t<Mapper>>
		Either<Lhs, right_type> mapLeft(Mapper&& mapper) && {
		    if (this->isRight()) {
			return Either<Lhs, right_type>::Right(std::move(this->unsafeGetRight()));
		    } else if (this->isLeft()) {
			return Either<Lhs, right_type>::Left(
			    invoke(std::forward<Mapper>(mapper), std::move(this->unsafeGetLeft())));
		    } else {
			return Either<Lhs, right_type>::valuelessInstance();
		    }
		}

		template <class Mapper, class Rhs = scl::meta::return_t<Mapper>>
		Either<left_type, Rhs> mapRight(Mapper&& mapper) const& {
		    if (this->isRight()) {
			return Either<left_type, Rhs>::Right(
			    invoke(std::forward<Mapper>(mapper), this->unsafeGetRight()));
		    } else if (this->isLeft()) {
			return Either<left_type, Rhs>::Left(this->unsafeGetLeft());
		    } else {
			return Either<left_type, Rhs>::valuelessInstance();
		    }
		}

		template <class Mapper, class Rhs = scl::meta::return_t<Mapper>>
		Either<left_type, Rhs> mapRight(Mapper&& mapper) && {
		    if (this->isRight()) {
			return Either<left_type, Rhs>::Right(invoke(
			    std::forward<Mapper>(mapper), std::move(this->unsafeGetRight())));
		    } else if (this->isLeft()) {
			return Either<left_type, Rhs>::Left(std::move(this->unsafeGetLeft()));
		    } else {
			return Either<left_type, Rhs>::valuelessInstance();
		    }
		}

		template <class LeftMapper, class RightMapper,
			  class Lhs = scl::meta::return_t<LeftMapper>,
			  class Rhs = scl::meta::return_t<RightMapper>>
		Either<Lhs, Rhs> map(LeftMapper&& leftMapper, RightMapper&& rightMapper) const& {
		    if (this->isLeft()) {
			return Either<Lhs, Rhs>::Left(
			    invoke(std::forward<LeftMapper>(leftMapper), this->unsafeGetLeft()));
		    } else if (this->isRight()) {
			return Either<Lhs, Rhs>::Right(
			    invoke(std::forward<RightMapper>(rightMapper), this->unsafeGetRight()));
		    } else {
			return Either<Lhs, Rhs>::valuelessInstance();
		    }
		}

		template <class LeftMapper, class RightMapper,
			  class Lhs = scl::meta::return_t<LeftMapper>,
			  class Rhs = scl::meta::return_t<RightMapper>>
		Either<Lhs, Rhs> map(LeftMapper&& leftMapper, RightMapper&& rightMapper) && {
		    if (this->isLeft()) {
			return Either<Lhs, Rhs>::Left(invoke(std::forward<LeftMapper>(leftMapper),
							     std::move(this->unsafeGetLeft())));
		    } else if (this->isRight()) {
			return Either<Lhs, Rhs>::Right(
			    invoke(std::forward<RightMapper>(rightMapper),
				   std::move(this->unsafeGetRight())));
		    } else {
			return Either<Lhs, Rhs>::valuelessInstance();
		    }
		}

		template <class LeftMapper,
			  class Lhs = typename scl::meta::return_t<LeftMapper>::left_type>
		Either<Lhs, right_type> flatMapLeft(LeftMapper&& leftMapper) const& {
		    if (this->isLeft()) {
			return invoke(std::forward<LeftMapper>(leftMapper), unsafeGetLeft());
		    } else if (this->isRight()) {
			return Either<Lhs, right_type>::Right(unsafeGetRight());
		    } else {
			return Either<Lhs, right_type>::valuelessInstance();
		    }
		}

		template <class LeftMapper,
			  class Lhs = typename scl::meta::return_t<LeftMapper>::left_type>
		Either<Lhs, right_type> flatMapLeft(LeftMapper&& leftMapper) && {
		    if (this->isLeft()) {
			return invoke(std::forward<LeftMapper>(leftMapper),
				      std::move(unsafeGetLeft()));
		    } else if (this->isRight()) {
			return Either<Lhs, right_type>::Right(std::move(unsafeGetRight()));
		    } else {
			return Either<Lhs, right_type>::valuelessInstance();
		    }
		}

		template <class RightMapper,
			  class Rhs = typename scl::meta::return_t<RightMapper>::right_type>
		Either<left_type, Rhs> flatMapRight(RightMapper&& rightMapper) const& {
		    if (this->isRight()) {
			return invoke(std::forward<RightMapper>(rightMapper), unsafeGetRight());
		    } else if (this->isLeft()) {
			return Either<left_type, Rhs>::Left(unsafeGetLeft());
		    } else {
			return Either<left_type, Rhs>::valuelessInstance();
		    }
		}

		template <class RightMapper,
			  class Rhs = typename scl::meta::return_t<RightMapper>::right_type>
		Either<left_type, Rhs> flatMapRight(RightMapper&& rightMapper) && {
		    if (this->isRight()) {
			return invoke(std::forward<RightMapper>(rightMapper),
				      std::move(unsafeGetRight()));
		    } else if (this->isLeft()) {
			return Either<left_type, Rhs>::Left(std::move(unsafeGetLeft()));
		    } else {
			return Either<left_type, Rhs>::valuelessInstance();
		    }
		}

		template <class LeftMapper, class RightMapper,
			  class Lhs = typename scl::meta::return_t<LeftMapper>::left_type,
			  class Rhs = typename scl::meta::return_t<RightMapper>::right_type>
		Either<Lhs, Rhs> flatMapRight(LeftMapper&& leftMapper,
					      RightMapper&& rightMapper) const& {
		    if (this->isRight()) {
			return invoke(std::forward<RightMapper>(rightMapper), unsafeGetRight());
		    } else if (this->isLeft()) {
			return invoke(std::forward<LeftMapper>(leftMapper), unsafeGetLeft());
		    } else {
			return Either<Lhs, Rhs>::valuelessInstance();
		    }
		}

		template <class LeftMapper, class RightMapper,
			  class Lhs = typename scl::meta::return_t<LeftMapper>::left_type,
			  class Rhs = typename scl::meta::return_t<RightMapper>::right_type>
		Either<Lhs, Rhs> flatMap(LeftMapper&& leftMapper, RightMapper&& rightMapper) && {
		    if (this->isRight()) {
			return invoke(std::forward<RightMapper>(rightMapper),
				      std::move(unsafeGetRight()));
		    } else if (this->isLeft()) {
			return invoke(std::forward<LeftMapper>(leftMapper),
				      std::move(unsafeGetLeft()));
		    } else {
			return Either<Lhs, Rhs>::valuelessInstance();
		    }
		}

		const left_type& leftOr(const left_type& defaultValue) const noexcept {
		    if (this->isLeft()) {
			return unsafeGetLeft();
		    } else {
			return defaultValue;
		    }
		}

		left_type&& leftOr(left_type defaultValue) && noexcept {
		    if (this->isLeft()) {
			return std::move(unsafeGetLeft());
		    } else {
			return std::move(defaultValue);
		    }
		}

		const right_type& rightOr(const right_type& defaultValue) const noexcept {
		    if (this->isRight()) {
			return unsafeGetRight();
		    } else {
			return defaultValue;
		    }
		}

		right_type&& rightOr(right_type defaultValue) && noexcept {
		    if (this->isRight()) {
			return std::move(unsafeGetRight());
		    } else {
			return std::move(defaultValue);
		    }
		}

#if !SCL_NO_EXCEPTIONS
		template <class E>
		const left_type& leftOrThrow(E&& err) const& {
		    if (this->isLeft()) {
			return unsafeGetLeft();
		    }

		    throw err;
		}

		template <class E>
		left_type&& leftOrThrow(E&& err) && {
		    if (this->isLeft()) {
			return std::move(unsafeGetLeft());
		    }

		    throw err;
		}

		template <class E>
		const right_type& rightOrThrow(E&& err) const& {
		    if (this->isRight()) {
			return unsafeGetRight();
		    }

		    throw err;
		}

		template <class E>
		right_type&& rightOrThrow(E&& err) && {
		    if (this->isRight()) {
			return std::move(unsafeGetRight());
		    }

		    throw err;
		}
#endif

	    protected:
		struct empty_t {};

		explicit Either(empty_t) {
		    this->leftIsInit = false;
		    this->valuelessByDestructor = true;
		}

		static Either valuelessInstance() {
		    return Either{empty_t{}};
		}

		left_type& unsafeGetLeft() & noexcept {
		    return *this->leftPtr();
		}

		const left_type& unsafeGetLeft() const& noexcept {
		    return *this->leftPtr();
		}

		left_type&& unsafeGetLeft() && noexcept {
		    return std::move(*this->leftPtr());
		}

		right_type& unsafeGetRight() & noexcept {
		    return *this->rightPtr();
		}

		const right_type& unsafeGetRight() const& noexcept {
		    return *this->rightPtr();
		}

		right_type&& unsafeGetRight() && noexcept {
		    return std::move(*this->rightPtr());
		}
	};
    }  // namespace utils
}  // namespace scl