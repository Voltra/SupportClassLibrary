#pragma once
#include <memory>
#include <new>

#include "./exchange.h"
#include "../meta/base/base.hpp"
#include "../meta/type_queries/copyable.h"
#include "../meta/type_queries/movable.h"

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

                    CopyableRawStorage(const Derived& other) noexcept(
                        scl::meta::is_nothrow_copy_constructible<T>()) {
                        auto* self = static_cast<Derived*>(this);
                        auto& rhs = static_cast<const Derived&>(other);

                        self->destructor();
                        self->initialize(scl::meta::as_const(rhs.get()));
                        self->init = rhs.init;
                    }

                    CopyableRawStorage& operator=(const Derived& other) noexcept(
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
                    bool init = false;

                    storage_type* rawPtr() { return &storage; }

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
                    void initialize(Args&&... args) noexcept(
                        scl::meta::is_nothrow_destructible<T>()
                        && scl::meta::is_nothrow_constructible<T, Args...>()) {
                        this->destructor();
                        new (this->rawPtr()) T{std::forward<Args>(args)...};
                        this->init = true;
                    }

                public:
                    RawStorageImpl() noexcept : storage{}, init{false} {}

                    RawStorageImpl(RawStorageImpl&& rhs) : RawStorageImpl() {
                        if (rhs.init) {
                            this->destructor();
                            this->initialize(std::move(rhs.get()));
                            this->init = exchange(rhs.init, false);
                        }
                    }

                    RawStorageImpl& operator=(RawStorageImpl&& rhs) {
                        this->destructor();

                        if (!rhs.init) {
                            this->init = false;
                        } else {
                            this->initialize(std::move(rhs.get()));
                            this->init = exchange(rhs.init, false);
                        }

                        return *this;
                    }

                    explicit RawStorageImpl(T&& value) : RawStorageImpl() {
                        this->reinit(std::move(value));
                    }

                    RawStorageImpl& operator=(T&& value) {
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
                    T& constructor(Args&&... args) {
                        this->initialize(std::forward<Args>(args)...);
                        return this->get();
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
                    RawStorageImpl& reset() {
                        this->destroy();
                        return *this;
                    }

                    template <class... Args>
                    void reinit(Args&&... args) {
                        this->destroy();
                        this->constructor(std::forward<Args>(args)...);
                    }

                    /**
                     * Access the underlying data
                     * @return a mutable reference to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T& get() & {
                        if (!this->init)
                            throw scl::exceptions::UninitializedMemoryAccess{};

                        return *this->ptr();
                    }

                    /**
                     * Access the underlying data
                     * @return a mutable reference to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T&& get() && {
                        if (!this->init)
                            throw scl::exceptions::UninitializedMemoryAccess{};

                        return std::move(*this->ptr());
                    }

                    /**
                     * Get the value from a constant RawStorage (e.g. w/ a constant class that uses
                     * the storage)
                     * @return an immutable reference to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    const T& get() const& {
                        if (!this->init)
                            throw scl::exceptions::UninitializedMemoryAccess{};

                        return *reinterpret_cast<const T*>(&this->storage);
                    }

                    /**
                     * Access the underlying data
                     * @return a mutable reference to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T& operator*() & { return this->get(); }

                    /**
                     * Get the value from a constant RawStorage (e.g. w/ a constant class that uses
                     * the storage)
                     * @return an immutable reference to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    const T& operator*() const& { return this->get(); }

                    /**
                     * Get the value from a constant RawStorage (e.g. w/ a constant class that uses
                     * the storage)
                     * @return an immutable reference to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T&& operator*() && { return this->get(); }

                    /**
                     * Get a pointer to the underlying data
                     * @return a mutable pointer to the underlying data
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    T* operator->() { return &(this->get()); }

                    /**
                     * Get a pointer to the value from a constant RawStorage (e.g. w/ a constant
                     * class that uses the storage)
                     * @return an immutable pointer to the underlying value
                     * @throws scl::exceptions::UninitializedMemoryAccess
                     */
                    scl::meta::real_const_t<T*> operator->() const { return &(this->get()); }

                    /**
                     * Determine whether or not the storage holds a value
                     * @return TRUE if it does, FALSE otherwise
                     */
                    bool hasValue() const { return this->init; }

                    /**
                     * Implicit conversion to bool
                     * @return TRUE if the storage holds a value, FALSE otherwise
                     */
                    operator bool() const { return this->hasValue(); }
            };
        }  // namespace details

        template <class T>
        struct RawStorage : public details::RawStorageImpl<T, RawStorage<T>> {
                using details::RawStorageImpl<T, RawStorage<T>>::RawStorageImpl;
                using details::RawStorageImpl<T, RawStorage<T>>::operator=;
        };
    }  // namespace utils
}  // namespace scl