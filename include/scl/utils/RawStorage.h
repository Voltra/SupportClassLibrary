#pragma once
#include <memory>
#include <new>

#include "../meta/meta.hpp"
#include "../exceptions/exceptions.hpp"
#include "./exchange.h"

namespace scl {
    namespace utils {
        namespace details {
            struct TriviallyDestructibleRawStorage {
                /**
                 * Call the destructor on the allocated object
                 * @warning If non initialized, it is a no-op
                 */
                void destructor() noexcept {}

                ~TriviallyDestructibleRawStorage() noexcept = default;
            };

            template <class T, class Derived>
            struct DestructibleRawStorage : public scl::meta::crtp_base<Derived> {
                /**
                 * Call the destructor on the allocated object
                 * @warning If non initialized, it is a no-op
                 */
                void destructor() noexcept(scl::meta::is_nothrow_destructible<T>()) {
                    auto&& self = this->self_();

                    if (self.init) {
                        self.ptr()->~T();
                        self.init = false;
                    }
                }

                ~DestructibleRawStorage() noexcept(scl::meta::is_nothrow_destructible<T>()) {
                    this->self_().destructor();
                }
            };

            template <class T, class Derived>
            using raw_storage_destructor_impl
                = scl::meta::conditional_t<scl::meta::is_trivially_destructible<T>(),
                                           TriviallyDestructibleRawStorage,
                                           DestructibleRawStorage<T, Derived>>;

            template <class T, class Derived>
            struct CopyableRawStorage : public scl::meta::copyable_base {
                CopyableRawStorage() = default;

                CopyableRawStorage(const CopyableRawStorage& other) noexcept(
                    scl::meta::is_nothrow_copy_constructible<T>()) {
                    auto* self = static_cast<Derived*>(this);
                    auto& rhs = static_cast<const Derived&>(other);

                    self->destructor();
                    self->initialize(scl::meta::as_const(rhs.get()));
                    self->init = rhs.init;
                }

                CopyableRawStorage& operator=(const CopyableRawStorage& other) noexcept(
                    scl::meta::is_nothrow_copyable<T>()) {
                    auto* self = static_cast<Derived*>(this);
                    auto& rhs = static_cast<const Derived&>(other);

                    self->destructor();
                    self->initialize(scl::meta::as_const(rhs.get()));
                    self->init = rhs.init;

                    return *this;
                }
            };

            template <class T, class Derived>
            using raw_storage_copy_impl = scl::meta::conditional_t<scl::meta::is_copyable<T>(),
                                                                   CopyableRawStorage<T, Derived>,
                                                                   scl::meta::non_copyable_base>;

            template <class T, class Derived>
            class RawStorageImpl : public raw_storage_destructor_impl<T, Derived>,
                                   public raw_storage_copy_impl<T, Derived> {
            public:
                friend CopyableRawStorage<T, Derived>;
                friend DestructibleRawStorage<T, Derived>;

                using base_type = RawStorageImpl;
                using value_type = scl::meta::remove_cv_ref_t<scl::meta::decay_t<T>>;

                using raw_storage_destructor_impl<T, Derived>::raw_storage_destructor_impl;
                using raw_storage_copy_impl<T, Derived>::raw_storage_copy_impl;
                using raw_storage_copy_impl<T, Derived>::operator=;

            protected:
                using storage_type
                    = scl::meta::aligned_storage_t<sizeof(value_type), alignof(value_type)>;

                /**
                 * @var storage
                 * The actual memory on the stack that can hold an object of type T
                 */
                storage_type storage;

                /**
                 * @var init
                 * Determines whether the object has been constructed
                 */
                bool init;

                constexpr storage_type* rawPtr() { return &storage; }

                T* ptr() { return reinterpret_cast<T*>(this->rawPtr()); }

                /**
                 * Construct the variable in the storage
                 * @tparam Args being the types of arguments passed to the constructor
                 * @param args being the arguments passed to the constructor
                 * @return a mutable reference to the constructed object
                 *
                 * @warning Uses placement new
                 * @warning Cleans the memory if there was data (calls the destructor)
                 */
                template <class... Args>
                constexpr void initialize(Args&&... args) noexcept(
                    scl::meta::is_nothrow_destructible<T>()
                    && scl::meta::is_nothrow_constructible<T, Args...>()) {
                    this->destructor();
                    new (this->rawPtr()) T{std::forward<Args>(args)...};
                    this->init = true;
                }

            public:
                constexpr RawStorageImpl() noexcept : storage{}, init{false} {}

                constexpr RawStorageImpl(RawStorageImpl&& other) : RawStorageImpl() {
                    if (other.init) {
                        this->destructor();
                        this->initialize(std::move(other.get()));
                        this->init = exchange(other.init, false);
                    }
                }

                constexpr explicit RawStorageImpl(T&& value) : RawStorageImpl() {
                    this->reinit(std::move(value));
                }

                constexpr RawStorageImpl& operator=(RawStorageImpl&& other) {
                    this->destructor();

                    if (!other.init) {
                        this->init = false;
                    } else {
                        this->initialize(std::move(other.get()));
                        this->init = exchange(other.init, false);
                    }

                    return *this;
                }

                constexpr RawStorageImpl& operator=(T&& value) {
                    this->reinit(std::move(value));
                    return *this;
                }

                /**
                 * Construct the variable in the storage
                 * @tparam Args being the types of arguments passed to the constructor
                 * @param args being the arguments passed to the constructor
                 * @return a mutable reference to the constructed object
                 *
                 * @warning Uses placement new
                 * @warning Cleans the memory if there was data (calls the destructor)
                 */
                template <class... Args>
                constexpr T& constructor(Args&&... args) {
                    this->initialize(std::forward<Args>(args)...);
                    return this->get();
                }

                /**
                 * Alias for RawStorage::constructor
                 */
                template <class... Args>
                constexpr T& construct(Args&&... args) {
                    return this->constructor(std::forward<Args&&>(args)...);
                }

                /**
                 * Alias for RawStorage::destructor
                 */
                constexpr void destroy() noexcept(scl::meta::is_nothrow_destructible<T>()) {
                    this->destructor();
                }

                /**
                 * Alias for RawStorage::destroy
                 * @return a reference to this RawStorage
                 */
                constexpr RawStorageImpl& reset() {
                    this->destroy();
                    return *this;
                }

                template <class... Args>
                constexpr void reinit(Args&&... args) {
                    this->destroy();
                    this->constructor(std::forward<Args>(args)...);
                }

                /**
                 * Access the underlying data
                 * @return a mutable reference to the underlying data
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr T& get() & {
                    if (!this->init) throw scl::exceptions::UninitializedMemoryAccess{};

                    return *this->ptr();
                }

                /**
                 * Access the underlying data
                 * @return a mutable reference to the underlying data
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr T&& get() && {
                    if (!this->init) throw scl::exceptions::UninitializedMemoryAccess{};

                    return std::move(*this->ptr());
                }

                /**
                 * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the
                 * storage)
                 * @return an immutable reference to the underlying value
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr const T& get() const& {
                    if (!this->init) throw scl::exceptions::UninitializedMemoryAccess{};

                    return *reinterpret_cast<const T*>(&this->storage);
                }

                /**
                 * Access the underlying data
                 * @return a mutable reference to the underlying data
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr T& operator*() & { return this->get(); }

                /**
                 * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the
                 * storage)
                 * @return an immutable reference to the underlying value
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr const T& operator*() const& { return this->get(); }

                /**
                 * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the
                 * storage)
                 * @return an immutable reference to the underlying value
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr T&& operator*() && { return this->get(); }

                /**
                 * Get a pointer to the underlying data
                 * @return a mutable pointer to the underlying data
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr T* operator->() { return &(this->get()); }

                /**
                 * Get a pointer to the value from a constant RawStorage (e.g. w/ a constant class
                 * that uses the storage)
                 * @return an immutable pointer to the underlying value
                 * @throws scl::exceptions::UninitializedMemoryAccess
                 */
                constexpr scl::meta::real_const_t<T*> operator->() const { return &(this->get()); }

                /**
                 * Determine whether or not the storage holds a value
                 * @return TRUE if it does, FALSE otherwise
                 */
                constexpr bool hasValue() const { return this->init; }

                /**
                 * Implicit conversion to bool
                 * @return TRUE if the storage holds a value, FALSE otherwise
                 */
                constexpr operator bool() const { return this->hasValue(); }
            };
        }  // namespace details

        template <class T>
        struct RawStorage : public details::RawStorageImpl<T, RawStorage<T>> {
            using details::RawStorageImpl<T, RawStorage<T>>::RawStorageImpl;
            using details::RawStorageImpl<T, RawStorage<T>>::operator=;
        };
    }  // namespace utils
}  // namespace scl