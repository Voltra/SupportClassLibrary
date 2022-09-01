#pragma once
#include <string>
#include <utility>

#include "../detect/version.h"
#include "../exceptions/EmptyOptionalAccess.h"
#include "../macros.h"
#include "../meta/base/base.hpp"
#include "../meta/type_aliases/as_const.h"
#include "../meta/type_aliases/stl.h"
#include "../meta/type_queries/stl.h"
#include "./RawStorage.h"
#include "./exchange.h"
#include "./invoke.h"
#include "./swap.h"

#ifdef SCL_CPP17
    #include <optional>
#endif

#if SCL_NO_EXCEPTIONS
    #warning \
	"scl::utils::Optional's get() is unsafe without exceptions. Make sure you check your object's lifetime (i.e. no use-after-move) and use hasValue()"
#endif

// TODO: more rvalue-qualified methods?

namespace scl {
    namespace utils {
	namespace details {
	    /**
	     * An empty class serving as the type of an empty Optional<T>
	     */
	    struct OptionalNone final {
		    constexpr bool operator==(OptionalNone) const { return true; }
		    constexpr bool operator!=(OptionalNone) const { return false; }
		    constexpr bool operator<(OptionalNone) const { return false; }
		    constexpr bool operator<=(OptionalNone) const { return true; }
		    constexpr bool operator>(OptionalNone) const { return false; }
		    constexpr bool operator>=(OptionalNone) const { return true; }
	    };

	    template <class T, class Derived>
	    struct OptionalEngineCopyBase : public scl::meta::crtp_base<Derived> {
		    OptionalEngineCopyBase() noexcept = default;

		    explicit OptionalEngineCopyBase(const Derived& rhs) noexcept(
			scl::meta::is_nothrow_copyable<T>()) {
			this->self_().payload = rhs.payload;
		    }

		    OptionalEngineCopyBase& operator=(const Derived& rhs) noexcept(
			scl::meta::is_nothrow_copyable<T>()) {
			this->self_().payload = rhs.payload;
			return *this;
		    }

		    explicit OptionalEngineCopyBase(const T& ref) noexcept(
			scl::meta::is_nothrow_copyable<T>()) {
			this->self_().payload = ref;
		    }
	    };

	    template <class T, class Derived>
	    using optional_copy_base = scl::meta::conditional_t<scl::meta::is_copyable<T>(),
								OptionalEngineCopyBase<T, Derived>,
								scl::meta::non_copyable_base>;

	    template <class T>
	    struct SclOptionalEngine : public optional_copy_base<T, SclOptionalEngine<T>> {
		    friend optional_copy_base<T, SclOptionalEngine<T>>;

		    using value_type = scl::meta::remove_cv_ref_t<T>;

		    using optional_copy_base<T, SclOptionalEngine<T>>::optional_copy_base;
		    using optional_copy_base<T, SclOptionalEngine<T>>::operator=;

		    /**
		     * @var payload
		     * A raw storage to hold an instance of the object
		     */
		    RawStorage<T> payload;

		    SclOptionalEngine() : payload{} {}
		    explicit SclOptionalEngine(OptionalNone) : SclOptionalEngine() {}
		    explicit SclOptionalEngine(T value) : payload{std::move(value)} {}

		    SclOptionalEngine(SclOptionalEngine&& rhs) noexcept(
			scl::meta::is_nothrow_movable<T>())
			: payload{std::move(rhs.payload)} {}

		    SclOptionalEngine& operator=(SclOptionalEngine&& rhs) noexcept(
			scl::meta::is_nothrow_movable<T>()) {
			swap(this->payload, rhs.payload);
			return *this;
		    }

		    SclOptionalEngine& operator=(OptionalNone) {
			this->payload.destroy();
			return *this;
		    }

		    SCL_NODISCARD bool hasValue() const { return this->payload.hasValue(); }

		    SCL_NODISCARD value_type& get() & noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
			if (!this->hasValue())
			    throw exceptions::EmptyOptionalAccess{};
#endif

			return this->payload.get();
		    }

		    SCL_NODISCARD const value_type& get() const& noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
			if (!this->hasValue())
			    throw exceptions::EmptyOptionalAccess{};
#endif

			return this->payload.get();
		    }

		    SCL_NODISCARD value_type&& get() && noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
			if (!this->hasValue())
			    throw exceptions::EmptyOptionalAccess{};
#endif

			return std::move(this->payload.get());
		    }

#define SCL_TPL                                                                                   \
    template <class U, class = scl::meta::enable_if_t<                                            \
			   !scl::meta::is_same<scl::meta::decay_t<U>, scl::meta::decay_t<T>>()    \
			   && !scl::meta::is_same<scl::meta::decay_t<U>, details::OptionalNone>() \
			   && !scl::meta::is_same<scl::meta::decay_t<U>, SclOptionalEngine>()>>
		    /**
		     * Implicit conversion universal ref constructor
		     * @tparam U being the type to implicitly convert from
		     * @param value being the value to construct from
		     */
		    SCL_TPL explicit SclOptionalEngine(U&& value) {
			*this = std::forward<U>(value);
		    }

		    /**
		     * Implicit conversion universal ref assignment
		     * @tparam U being the type to implicitly convert from
		     * @param value being the value to assign from
		     * @return a reference to this Optional
		     */
		    SCL_TPL SclOptionalEngine& operator=(U&& value) {
			this->payload.construct(std::forward<U>(value));
			return *this;
		    }
#undef SCL_TPL
	    };

#ifdef SCL_CPP17
	    template <class T>
	    struct StdOptionalEngine : public optional_copy_base<T, StdOptionalEngine<T>>,
				       public OptionalEngineMoveBase<T, SclOptionalEngine<T>> {
		    using value_type = scl::meta::remove_cv_ref_t<T>;

		    using optional_copy_base<T, StdOptionalEngine<T>>::optional_copy_base;
		    using OptionalEngineMoveBase<T, StdOptionalEngine<T>>::OptionalEngineMoveBase;
		    using optional_copy_base<T, StdOptionalEngine<T>>::operator=;
		    using OptionalEngineMoveBase<T, StdOptionalEngine<T>>::operator=;

		    std::optional<T> payload;

		    StdOptionalEngine() : payload{} {}

		    StdOptionalEngine(OptionalNone) : StdOptionalEngine() {}

		    explicit StdOptionalEngine(T value) : payload{std::move(value)} {}

		    StdOptionalEngine& operator=(OptionalNone) {
			this->payload.reset();
			return *this;
		    }

		    SCL_NODISCARD bool hasValue() const { return payload.has_value(); }

		    SCL_NODISCARD value_type& get() & noexcept(SCL_NO_EXCEPTIONS) {
    #if !SCL_NO_EXCEPTIONS
			if (!this->hasValue())
			    throw exceptions::EmptyOptionalAccess{};
    #endif

			return this->payload.value();
		    }

		    SCL_NODISCARD const value_type& get() const& noexcept(SCL_NO_EXCEPTIONS) {
    #if !SCL_NO_EXCEPTIONS
			if (!this->hasValue())
			    throw exceptions::EmptyOptionalAccess{};
    #endif

			return this->payload.value();
		    }

		    SCL_NODISCARD value_type&& get() && noexcept(SCL_NO_EXCEPTIONS) {
    #if !SCL_NO_EXCEPTIONS
			if (!this->hasValue())
			    throw exceptions::EmptyOptionalAccess{};
    #endif

			return std::move(this->payload.value());
		    }

    #define SCL_TPL                                                                       \
	template <class U,                                                                \
		  class = scl::meta::enable_if_t<                                         \
		      !scl::meta::is_same<scl::meta::decay_t<U>, scl::meta::decay_t<T>>() \
		      && !scl::meta::is_same<scl::meta::decay_t<U>, None>()               \
		      && !scl::meta::is_same<scl::meta::decay_t<U>, StdOptionalEngine>()>>
		    /**
		     * Implicit conversion universal ref constructor
		     * @tparam U being the type to implicitly convert from
		     * @param value being the value to construct from
		     */
		    TPL explicit StdOptionalEngine(U&& value) noexcept(
			scl::meta::is_nothrow_constructible<T, U>()) {
			*this = std::forward<U&&>(value);
		    }

		    /**
		     * Implicit conversion universal ref assignment
		     * @tparam U being the type to implicitly convert from
		     * @param value being the value to assign from
		     * @return a reference to this Optional
		     */
		    TPL StdOptionalEngine& operator=(U&& value) noexcept(
			scl::meta::is_nothrow_constructible<T, U>()) {
			this->payload = std::forward<U&&>(value);
			return *this;
		    }
    #undef SCL_TPL
	    };
#else
	    template <class T>
	    using StdOptionalEngine = SclOptionalEngine<T>;
#endif

	    template <class T>
	    using DefaultOptionalEngine = scl::meta::conditional_t<scl::detect::version::cpp17,
								   details::StdOptionalEngine<T>,
								   details::SclOptionalEngine<T>>;
	}  // namespace details

	constexpr details::OptionalNone none{};

	template <class T, class OptionalEngine = details::DefaultOptionalEngine<T>>
	class Optional : public OptionalEngine {
	    public:
		using engine_type = OptionalEngine;

		using value_type = typename engine_type::value_type;

		using OptionalEngine::OptionalEngine;
		using OptionalEngine::operator=;
		using OptionalEngine::get;
		using OptionalEngine::hasValue;

		template <class Value, class Engine>
		friend class Optional;

	    protected:
		template <class... Args>
		Optional(SCL_INPLACE_T, Args&&... args) : Optional() {
		    OptionalEngine::payload.construct(std::forward<Args>(args)...);
		}

	    public:
		/**
		 * Construct an optional from a pointer
		 * @param ptr being the pointer to construct from
		 * @return an empty optional if ptr is null, an initialized pointer otherwise
		 */
		static Optional fromPointer(const T* ptr) { return !ptr ? none : *ptr; }

		static Optional fromPointer(std::nullptr_t) { return none; }

		static Optional ownFromPointer(T* ptr) { return !ptr ? none : std::move(*ptr); }

		/**
		 * Construct an optional inplace
		 * @tparam Args being the types of the arguments for the constructor
		 * @param args being the arguments for the constructor
		 * @return an initialized optional
		 */
		template <class... Args>
		static Optional inplace(Args&&... args) {
		    return Optional{SCL_INPLACE, std::forward<Args>(args)...};
		}

		/**
		 * Construct an optional from a reference to an object
		 * @param ref being the reference to construct from
		 * @return an initialized optional
		 */
		static Optional ref(const value_type& ref) {
		    Optional ret = ref;
		    return ret;
		}

		Optional(details::OptionalNone) noexcept : Optional() {}

		/**
		 * A semantic alias for Optional<T>::get
		 * @return a const& to the stored value
		 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
		 */
		SCL_NODISCARD const value_type& value() const { return this->get(); }

		/**
		 * A semantic alias for Optional<T>::get
		 * @return a ref to the stored value
		 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
		 */
		SCL_NODISCARD value_type& value() { return this->get(); }

		/**
		 * Get an immutable pointer to the contained value
		 * @return a realConst(T*) to the value
		 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
		 */
		scl::meta::real_const_t<value_type*> ptr() const { return &(this->get()); }

		/**
		 * Get a pointer to the contained value
		 * @return a pointer to the value
		 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
		 */
		value_type* ptr() { return &(this->get()); }

		/**
		 * Automatic bool conversion
		 * @return TRUE if there's a value, FALSE otherwise
		 */
		SCL_NODISCARD operator bool() const { return this->hasValue(); }

		/**
		 * Alias for scl::utils::Optional::get
		 * @return A const reference to the optional's value
		 */
		SCL_NODISCARD const value_type& operator*() const& { return this->get(); }

		/**
		 * Alias for scl::utils::Optional::get
		 * @return A const reference to the optional's value
		 */
		SCL_NODISCARD value_type& operator*() & { return this->get(); }

		/**
		 * Alias for scl::utils::Optional::get
		 * @return A const reference to the optional's value
		 */
		SCL_NODISCARD value_type&& operator*() && { return this->get(); }

		/**
		 * Retrieves the value if there's one or return the default value provided
		 * @param defaultValue being the value to return if there's no values
		 * @return a const reference to the stored value
		 */
		const value_type& orElse(const value_type& defaultValue) const {
		    return this->hasValue() ? this->get() : defaultValue;
		}

		/**
		 * Calls a function if the value is present
		 * @tparam F being the function's type
		 * @param f being the function to call
		 * @return a reference to this optional (for chaining purposes)
		 */
		template <class F>
		const Optional& doIfPresent(F&& f) const {
		    if (this->hasValue()) {
			invoke(std::forward<F>(f), this->get());
		    }

		    return *this;
		}

		/**
		 * Calls a function if the value is present
		 * @tparam F being the function's type
		 * @param f being the function to call
		 * @return a reference to this optional (for chaining purposes)
		 */
		template <class F>
		Optional& doIfPresent(F&& f) {
		    if (this->hasValue()) {
			invoke(std::forward<F>(f), this->get());
		    }

		    return *this;
		}

		/**
		 * Alias for scl::utils::Optional::doIfPresent
		 */
		template <class F>
		const Optional& ifSome(F&& f) const {
		    return this->doIfPresent(std::forward<F>(f));
		}

		/**
		 * Alias for scl::utils::Optional::doIfPresent
		 */
		template <class F>
		Optional& ifSome(F&& f) {
		    return this->doIfPresent(std::forward<F>(f));
		}

		/**
		 * Alias for scl::utils::Optional::doIfPresent
		 */
		template <class F>
		Optional& peek(F&& callback) {
		    return this->doIfPresent(std::forward<F>(callback));
		}

		/**
		 * Alias for scl::utils::Optional::doIfPresent
		 */
		template <class F>
		const Optional& peek(F&& callback) const {
		    return this->doIfPresent(std::forward<F>(callback));
		}

		/**
		 * Calls a function if there is no value
		 * @tparam F being the function's type
		 * @param f being the function to call
		 * @return a reference to this optional (for chaining purposes)
		 */
		template <class F>
		const Optional& doIfEmpty(F&& f) const {
		    if (!this->hasValue())
			std::forward<F>(f)();

		    return *this;
		}

		/**
		 * Calls a function if there is no value
		 * @tparam F being the function's type
		 * @param f being the function to call
		 * @return a reference to this optional (for chaining purposes)
		 */
		template <class F>
		Optional& doIfEmpty(F&& f) {
		    if (!this->hasValue())
			std::forward<F>(f)();

		    return *this;
		}

		/**
		 * Alias for scl::utils::Optional::doIfEmpty
		 */
		template <class F>
		const Optional& ifNone(F&& f) const {
		    return this->doIfEmpty(std::forward<F>(f));
		}

		/**
		 * Alias for scl::utils::Optional::doIfEmpty
		 */
		template <class F>
		Optional& ifNone(F&& f) {
		    return this->doIfEmpty(std::forward<F>(f));
		}

#if !SCL_NO_EXCEPTIONS

		/**
		 * Tries to retrieve the value, throws the given exception if there's none
		 * @tparam E being the exception type
		 * @param e being the exception to throw if there's no value
		 * @return a const& to the value
		 */
		template <class E>
		const value_type& orThrow(const E& e) const {
		    if (this->hasValue())
			return this->get();

		    throw e;
		}

		/**
		 * Tries to retrieve the value, throws the given exception if there's none
		 * @tparam E being the exception type
		 * @param e being the exception to throw if there's no value
		 * @return a ref to the value
		 */
		template <class E>
		value_type& orThrow(const E& e) {
		    if (this->hasValue())
			return this->get();

		    throw e;
		}
#endif

		/**
		 * Maps this Optional<T> to an Optional<U> via the provided mapper function (T -> U)
		 * @tparam F being the type of the mapper function (auto deduction)
		 * @tparam U being the value type for the mapped optional (auto deduction from F)
		 * @param mapper being the mapper function to use to map values
		 * @return an Optional<U>
		 */
		template <class F, class U = scl::meta::return_t<F>>
		Optional<U> map(F&& mapper) const {
		    if (this->hasValue()) {
			const value_type& x = this->get();
			auto&& ret = invoke(mapper, x);
			return Optional<U>{std::forward<U>(ret)};
		    }

		    return none;
		}

		/**
		 * Alias for Optional::map
		 */
		template <class F, class U = scl::meta::return_t<F>>
		Optional<U> mapTo(F&& mapper) const {
		    return this->map<F, U>(mapper);
		}

		/**
		 * Filters the value according to the given predicate
		 * @tparam F being the type of predicate (auto deduction)
		 * @param predicate being the predicate used to determine whether or not it should
		 * keep the value
		 * @return a new optional that might not contain the original value
		 */
		template <class F>
		const Optional<T>& filter(F predicate) const& {
		    if (this->hasValue()) {
			const value_type& x = this->get();
			return invoke(predicate, x) ? *this : Optional<T>{};
		    }

		    return none;
		}

		/**
		 * Filters the value according to the given predicate
		 * @tparam F being the type of predicate (auto deduction)
		 * @param predicate being the predicate used to determine whether or not it should
		 * keep the value
		 * @return a new optional that might not contain the original value
		 */
		template <class F>
		Optional<T> filter(F predicate) && {
		    if (this->hasValue()) {
			T&& x = this->get();
			return invoke(predicate, scl::meta::as_const(x)) ? Optional<T>{std::move(x)}
									 : none;
		    }

		    return none;
		}

		/**
		 * Flat maps this optional to an optional of another type
		 * @tparam F being the type of the mapper function (auto deduction)
		 * @tparam U being the value type for the mapped optional (auto deduction from F)
		 * @param mapper being the mapper function
		 * @return the mapped optional
		 */
		template <class F, class U = typename scl::meta::return_t<F>::value_type>
		Optional<U> flatMap(F&& mapper) const {
		    return this->hasValue() ? invoke(mapper, this->get()) : none;
		}

		/**
		 * Alias for Optional::flatMap
		 */
		template <class F, class U = typename scl::meta::return_t<F>::value_type>
		Optional<U> flatMapTo(F&& mapper) const {
		    return this->flatMap<F, U>(std::forward<F>(mapper));
		}

		inline bool operator==(details::OptionalNone) const { return !this->hasValue(); }
		inline friend bool operator==(details::OptionalNone, const Optional& o) {
		    return o == none;
		}

		inline bool operator<(details::OptionalNone) const { return false; }
		inline friend bool operator<(details::OptionalNone, const Optional&) {
		    return true;
		}

		inline bool operator<=(details::OptionalNone) const { return (*this) == none; }
		inline friend bool operator<=(details::OptionalNone, const Optional&) {
		    return true;
		}

		inline bool operator>(details::OptionalNone) const { return true; }
		inline friend bool operator>(details::OptionalNone, const Optional&) {
		    return false;
		}

		inline bool operator>=(details::OptionalNone) const { return true; }
		inline friend bool operator>=(details::OptionalNone, const Optional& o) {
		    return o <= none;
		}

		inline bool operator!=(details::OptionalNone) const { return !((*this) == none); }
		inline friend bool operator!=(details::OptionalNone, const Optional& o) {
		    return o != none;
		}
	};
    }  // namespace utils
}  // namespace scl

namespace std {
    std::string to_string(const scl::utils::details::OptionalNone&) {
	return "[none ; scl::utils::details::OptionalNone]";
    }

    template <class T, class OptionalEngine,
	      class = scl::meta::void_enable_if_t<
		  scl::meta::exists<decltype(std::to_string(std::declval<const T&>()))>()>>
    std::string to_string(const scl::utils::Optional<T, OptionalEngine>& optional) {
	if (optional.hasValue()) {
	    return to_string(*optional);
	} else {
	    return to_string(scl::utils::none);
	}
    }
}  // namespace std