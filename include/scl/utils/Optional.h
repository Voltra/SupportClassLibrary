#pragma once
#include <string>
#include <utility>

#include "../detect/detect.hpp"
#include "../exceptions/EmptyOptionalAccess.h"
#include "../macros.h"
#include "../meta/meta.hpp"
#include "./RawStorage.h"
#include "./exchange.h"

#ifdef SCL_CPP17
    #include <optional>
#endif

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

            template <class T>
            struct SclOptionalEngine {
                using value_type = scl::meta::remove_cv_ref_t<T>;

                /**
                 * @var payload
                 * A raw storage to hold an instance of the object
                 */
                RawStorage<T> payload;

                SclOptionalEngine() : payload{} {}
                explicit SclOptionalEngine(OptionalNone) : SclOptionalEngine() {}
                explicit SclOptionalEngine(T value) : payload{std::move(value)} {}
                explicit SclOptionalEngine(T& ref) : payload{std::ref(ref)} {}
                SclOptionalEngine(const StdOptionalEngine<T>& rhs) : payload{rhs.payload} {}
                SclOptionalEngine(StdOptionalEngine<T>&& rhs) noexcept : payload{rhs.payload} {
                    rhs.payload.destroy();
                }

                SclOptionalEngine& operator=(const SclOptionalEngine<T>& rhs) {
                    this->payload = rhs.payload;
                    return *this;
                }

                SclOptionalEngine& operator=(SclOptionalEngine<T>&& rhs) noexcept {
                    this->payload = std::move(rhs.payload);
                    rhs.payload.destroy();
                    return *this;
                }

                SclOptionalEngine& operator=(OptionalNone) { this->payload.destroy(); }

                SCL_NODISCARD bool hasValue() const { return this->payload.hasValue(); }

                SCL_NODISCARD value_type& get() {
                    if (!this->hasValue()) throw exceptions::EmptyOptionalAccess{};

                    return this->payload.get();
                }

                SCL_NODISCARD const value_type& get() const {
                    if (!this->hasValue()) throw exceptions::EmptyOptionalAccess{};

                    return this->payload.get();
                }

#define SCL_TPL                                                                                \
    template <class U, class = scl::meta::enable_if_t<                                         \
                           !scl::meta::is_same<scl::meta::decay_t<U>, scl::meta::decay_t<T>>() \
                           && !scl::meta::is_same<scl::meta::decay_t<U>, None>()               \
                           && !scl::meta::is_same<scl::meta::decay_t<U>, SclOptionalEngine>()>>
                /**
                 * Implicit conversion universal ref constructor
                 * @tparam U being the type to implicitly convert from
                 * @param value being the value to construct from
                 */
                SCL_TPL SclOptionalEngine(U&& value) { *this = std::forward<U&&>(value); }

                /**
                 * Implicit conversion universal ref assignment
                 * @tparam U being the type to implicitly convert from
                 * @param value being the value to assign from
                 * @return a reference to this Optional
                 */
                SCL_TPL SclOptionalEngine& operator=(U&& value) {
                    this->payload.construct(std::forward<U&&>(value));
                    return *this;
                }
#undef SCL_TPL
            };

#ifdef SCL_CPP17
            template <class T>
            struct StdOptionalEngine {
                using value_type = scl::meta::remove_cv_ref_t<T>;

                std::optional<T> opt;

                StdOptionalEngine() : opt{} {}

                StdOptionalEngine(OptionalNone) : StdOptionalEngine() {}

                explicit StdOptionalEngine(T value) : opt{std::move(value)} {}

                explicit StdOptionalEngine(T& ref) : opt{std::ref(ref)} {}

                StdOptionalEngine(const StdOptionalEngine& rhs) : opt{rhs.opt} {}

                StdOptionalEngine(StdOptionalEngine&& rhs) noexcept : opt{exchange(rhs.opt, {})} {}

                StdOptionalEngine& operator=(const StdOptionalEngine& rhs) {
                    this->opt = rhs.opt;
                    return *this;
                }

                StdOptionalEngine& operator=(StdOptionalEngine&& rhs) {
                    this->opt = exchange(rhs.opt, {});
                }

                StdOptionalEngine& operator=(OptionalNone) { this->opt.reset(); }

                SCL_NODISCARD bool hasValue() const { return opt.has_value(); }

                SCL_NODISCARD value_type& get() {
                    if (!this->hasValue()) {
                        throw scl::exceptions::EmptyOptionalAccess{};
                    }

                    return this->opt.value();
                }

                SCL_NODISCARD const value_type& get() const {
                    if (!this->hasValue()) {
                        throw scl::exceptions::EmptyOptionalAccess{};
                    }

                    return this->opt.value();
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
                TPL StdOptionalEngine(U&& value) { *this = std::forward<U&&>(value); }

                /**
                 * Implicit conversion universal ref assignment
                 * @tparam U being the type to implicitly convert from
                 * @param value being the value to assign from
                 * @return a reference to this Optional
                 */
                TPL StdOptionalEngine& operator=(U&& value) {
                    this->opt = std::forward<U&&>(value);
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
        class Optional {
        public:
            using engine_type = OptionalEngine;  // TODO: Make this dynamic later

            using value_type = typename engine_type::value_type;

        protected:
            /**
             * @var engine
             * The implementation details
             */
            engine_type engine;

        public:

            /**
             * Construct an optional from a pointer
             * @param ptr being the pointer to construct from
             * @return an empty optional if ptr is null, an initialized pointer otherwise
             */
            constexpr static Optional fromPointer(const T* ptr) {
                return !ptr ? Optional{none} : Optional{*ptr};
            }

            constexpr static Optional fromPointer(std::nullptr_t) { return none; }

            /**
             * Construct an optional inplace
             * @tparam Args being the types of the arguments for the constructor
             * @param args being the arguments for the constructor
             * @return an initialized optional
             */
            template <class... Args>
            constexpr static Optional inplace(Args&&... args) {
                Optional ret = value_type{std::forward<Args>(args)...};
                return ret;
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

            /**
             * Default construct (no value)
             */
            Optional() : engine{} {}

            /**
             * Creates a non empty optional with the given value (copy)
             * @param value being the value to assign from
             */
            Optional(const value_type& value) : engine{value} {}

            /**
             * Assign a value to this optional
             * @param value being the value taken by the optional
             * @return a reference to this Optional
             */
            Optional& operator=(const value_type& value) {
                this->engine = value;
                return *this;
            }

            /**
             * Copy constructor
             * @param o being the Optional to copy from
             */
            Optional(const Optional& o) : engine{o.engine} {};

            /**
             * Copy assignment operator
             * @param rhs being the Optional to assign from
             * @return a reference to this Optional<T>
             */
            Optional& operator=(const Optional& rhs) {
                this->engine = rhs.engine;
                return *this;
            };

            /**
             * Move constructor
             * @param rhs being the Optional to construct from
             *
             * @warning moved-from Optional are in empty state
             * (i.e. equivalent to one constructed from none)
             */
            Optional(Optional&& rhs) noexcept : engine{std::move(rhs.engine)} {};

            /**
             * Move assignment operator
             * @param rhs being the Optional to assign from
             * @return a reference to this Optional<T>
             *
             * @warning moved-from Optional are in empty state
             * (i.e. equivalent to one constructed from none)
             */
            Optional& operator=(Optional&& rhs) noexcept {
                this->engine = std::move(rhs.engine);
                return *this;
            };

            /**
             * Instantiate an optional via an instance of None
             */
            Optional(details::OptionalNone) : Optional() {}

            /**
             * Assign from None
             * @return a reference to this Optional
             */
            Optional& operator=(details::OptionalNone) {
                this->engine.reset();
                return *this;
            }

#define SCL_TPL                                                                                \
    template <class U, class = scl::meta::enable_if_t<                                         \
                           !scl::meta::is_same<scl::meta::decay_t<U>, scl::meta::decay_t<T>>() \
                           && !scl::meta::is_same<scl::meta::decay_t<U>, None>()               \
                           && !scl::meta::is_same<scl::meta::decay_t<U>, Optional>()>>

            /**
             * Implicit conversion universal ref constructor
             * @tparam U being the type to implicitly convert from
             * @param value being the value to construct from
             */
            TPL Optional(U&& value) : engine{std::forward<U&&>(value)} {}

            /**
             * Implicit conversion universal ref assignment
             * @tparam U being the type to implicitly convert from
             * @param value being the value to assign from
             * @return a reference to this Optional
             */
            TPL Optional& operator=(U&& value) {
                this->engine = std::forward<U&&>(value);
                return *this;
            }
#undef SCL_TPL

            /**
             * Determines whether or not this Optional<T> is empty
             * @return TRUE if there's a value, FALSE otherwise
             */
            SCL_NODISCARD bool hasValue() const { return this->engine.hasValue(); }

            /**
             * Retrieves the value stored in this Optional<T>
             * @return a ref to the value stored
             * @throws scl::exceptions::EmptyOptionalAccess if there's no value
             */
            SCL_NODISCARD value_type& get() { return this->engine.get(); }

            /**
             * Retrieves the value stored in this Optional<T>
             * @return a const& to the value stored
             * @throws scl::exceptions::EmptyOptionalAccess if there's no value
             */
            SCL_NODISCARD const value_type& get() const { return this->engine.get(); }

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
        };
    }  // namespace utils
}  // namespace scl

namespace std {
    constexpr std::string to_string(const scl::utils::details::OptionalNone&) {
        return "[none ; scl::utils::details::OptionalNone]";
    }

    template <class T, class OptionalEngine, class = scl::meta::void_enable_if_t<
        scl::meta::exists<decltype(std::to_string(std::declval<const scl::utils::Optional<T, OptionalEngine>&>()))>()
    >>
    constexpr std::string to_string(const scl::utils::Optional<T, OptionalEngine>& optional) {
        if (optional.hasValue()) {
            return to_string(*optional);
        } else {
            return to_string(scl::utils::none);
        }
    }
}  // namespace std