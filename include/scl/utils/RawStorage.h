#pragma once
#include <memory>
#include <new>

#include "../alias/byte.h"
#include "../exceptions/UninitializedMemoryAccess.h"
#include "../meta/base/base.hpp"
#include "../meta/type_aliases/as_const.h"
#include "../meta/type_queries/copyable.h"
#include "../meta/type_queries/movable.h"
#include "./AlignedStorage.h"
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
            struct CopyableRawStorage : scl::meta::copyable_base {
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
                    friend raw_storage_destructor_impl<T, Derived>;
                    friend raw_storage_copy_impl<T, Derived>;

                    using base_type = RawStorageImpl;
                    using value_type = scl::meta::remove_cv_ref_t<scl::meta::decay_t<T>>;

                    using raw_storage_destructor_impl<T, Derived>::raw_storage_destructor_impl;
                    using raw_storage_copy_impl<T, Derived>::raw_storage_copy_impl;
                    using raw_storage_copy_impl<T, Derived>::operator=;

                protected:
                    using storage_type = scl::utils::AlignedStorage<value_type>;

                    /**
                     * @var storage
                     * The actual memory on the stack that can hold an object of type T
                     */
                    storage_type storage;

                    /**
                     * @var init
                     * Determines whether the object has been constructed
                     */
                    bool init = false;

                    void* rawPtr() { return storage; }
                    const void* rawPtr() const { return storage; }

                    T* ptr() & { return static_cast<T*>(this->rawPtr()); }

                    T* ptr() && { return static_cast<T*>(this->rawPtr()); }

                    const T* ptr() const& { return static_cast<const T*>(this->rawPtr()); }

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
                    void initialize(Args&&... args) noexcept(
                        scl::meta::is_nothrow_destructible<T>()
                        && scl::meta::is_nothrow_constructible<T, Args...>()) {
                        this->destructor();
                        new (this->rawPtr()) T{std::forward<Args>(args)...};
                        this->init = true;
                    }

                public:
                    RawStorageImpl() noexcept : storage{}, init{false} {}

                    RawStorageImpl(RawStorageImpl&& rhs) noexcept(
                        scl::meta::is_nothrow_movable<T>())
                        : RawStorageImpl() {
                        if (rhs.init) {
                            this->initialize(std::move(rhs.unsafeGet()));
                            rhs.init = false;
                        }
                    }

                    RawStorageImpl& operator=(RawStorageImpl&& rhs) noexcept(
                        scl::meta::is_nothrow_movable<T>()) {
                        if (!rhs.init) {
                            this->destructor();
                            this->init = false;
                        } else {
                            this->initialize(std::move(rhs.unsafeGet()));
                            rhs.init = false;
                        }

                        return *this;
                    }

                    explicit RawStorageImpl(T&& value) noexcept : RawStorageImpl() {
                        this->reinit(std::move(value));
                    }

                    RawStorageImpl& operator=(T&& value) noexcept {
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
                    T& constructor(Args&&... args) noexcept(
                        scl::meta::is_nothrow_destructible<T>()
                        && scl::meta::is_nothrow_constructible<T, Args...>()) {
                        this->initialize(std::forward<Args>(args)...);
                        return this->unsafeGet();
                    }

                    /**
                     * Alias for RawStorage::constructor
                     */
                    template <class... Args>
                    T& construct(Args&&... args) {
                        return this->constructor(std::forward<Args&&>(args)...);
                    }

                    /**
                     * Alias for RawStorage::destructor
                     */
                    void destroy() noexcept(scl::meta::is_nothrow_destructible<T>()) {
                        this->destructor();
                    }

                    /**
                     * Alias for RawStorage::destroy
                     * @return a reference to this RawStorage
                     */
                    RawStorageImpl& reset() noexcept(scl::meta::is_nothrow_destructible<T>()) {
                        this->destroy();
                        return *this;
                    }

                    template <class... Args>
                    void reinit(Args&&... args) noexcept(
                        scl::meta::is_nothrow_destructible<T>()
                        && scl::meta::is_nothrow_constructible<T, Args...>()) {
                        this->destroy();
                        this->initialize(std::forward<Args>(args)...);
                    }

                    T& unsafeGet() & noexcept { return *this->ptr(); }

                    T&& unsafeGet() && noexcept { return std::move(*this->ptr()); }

                    const T& unsafeGet() const& noexcept { return *this->ptr(); }

                    /**
                     * Access the underlying data
                     * @return a mutable reference to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T& get() & noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
                        if (!this->init)
                            throw scl::exceptions::UninitializedMemoryAccess{};
#endif

                        return unsafeGet();
                    }

                    /**
                     * Access the underlying data
                     * @return a mutable reference to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T&& get() && noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
                        if (!this->init)
                            throw scl::exceptions::UninitializedMemoryAccess{};
#endif

                        return unsafeGet();
                    }

                    /**
                     * Get the value from a constant RawStorage (e.g. w/ a constant class that uses
                     * the storage)
                     * @return an immutable reference to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    const T& get() const& noexcept(SCL_NO_EXCEPTIONS) {
#if !SCL_NO_EXCEPTIONS
                        if (!this->init)
                            throw scl::exceptions::UninitializedMemoryAccess{};
#endif

                        return unsafeGet();
                    }

                    /**
                     * Access the underlying data
                     * @return a mutable reference to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T& operator*() & noexcept(SCL_NO_EXCEPTIONS) { return this->get(); }

                    /**
                     * Get the value from a constant RawStorage (e.g. w/ a constant class that uses
                     * the storage)
                     * @return an immutable reference to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    const T& operator*() const& noexcept(SCL_NO_EXCEPTIONS) { return this->get(); }

                    /**
                     * Get the value from a constant RawStorage (e.g. w/ a constant class that uses
                     * the storage)
                     * @return an immutable reference to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T&& operator*() && noexcept(SCL_NO_EXCEPTIONS) { return this->get(); }

                    /**
                     * Get a pointer to the underlying data
                     * @return a mutable pointer to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T* operator->() noexcept(SCL_NO_EXCEPTIONS) { return &(this->get()); }

                    /**
                     * Get a pointer to the value from a constant RawStorage (e.g. w/ a constant
                     * class that uses the storage)
                     * @return an immutable pointer to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    scl::meta::real_const_t<T*> operator->() const noexcept(SCL_NO_EXCEPTIONS) {
                        return &(this->get());
                    }

                    /**
                     * Determine whether or not the storage holds a value
                     * @return TRUE if it does, FALSE otherwise
                     */
                    bool hasValue() const noexcept { return this->init; }

                    /**
                     * Implicit conversion to bool
                     * @return TRUE if the storage holds a value, FALSE otherwise
                     */
                    operator bool() const noexcept { return this->hasValue(); }
            };
        }  // namespace details

        template <class T>
        struct RawStorage : public details::RawStorageImpl<T, RawStorage<T>> {
                using details::RawStorageImpl<T, RawStorage<T>>::RawStorageImpl;
                using details::RawStorageImpl<T, RawStorage<T>>::operator=;
        };
    }  // namespace utils
}  // namespace scl