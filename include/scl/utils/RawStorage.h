#pragma once
#include "../meta/meta.hpp"
#include "./exchange.h"

namespace scl {
    namespace utils {
        template <class T>
        class RawStorage {
        protected:
            using storage_type = scl::meta::aligned_storage_t<sizeof(T), alignof(T)>;

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

            storage_type* rawPtr() { return &storage; }

            T* ptr() { return reinterpret_cast<T*>(this->rawPtr()); }

        public:
            RawStorage() : storage{}, init{false} {}

            RawStorage(RawStorage&& other) noexcept : RawStorage() {
                this->destructor();
                this->constructor(std::move(other.get()));
                this->init = exchange(other.init, false);
            }

            RawStorage& operator=(RawStorage&& other) noexcept {
                this->destructor();
                this->constructor(std::move(other.get()));
                this->init = exchange(other.init, false);
            }

            ~RawStorage() { this->destructor(); }

            /**
             * Construct the variable in the storage
             * @tparam Args being the types of arguments passed to the constructor
             * @param args being the arguments passed to the constructor
             * @return a mutable reference to the constructed object
             *
             * @warning Uses placement new
             * @warning Cleans the memory if there was data (calls the destrutor)
             */
            template <class... Args>
            T& constructor(Args&&... args) {
                this->destructor();
                new (rawPtr()) T{std::forward<Args>(args)...};
                this->init = true;
                return this->get();
            }

            /**
             * Call the destructor on the allocated object
             * @warning If non initialized, it is a no-op
             */
            void destructor() {
                if (this->init) {
                    this->ptr()->~T();
                    this->init = false;
                }
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
            void destroy() { this->destructor(); }

            /**
             * Alias for RawStorage::destroy
             * @return a reference to this RawStorage
             */
            RawStorage& reset() {
                this->destroy();
                return *this;
            }

            /**
             * Access the underlying data
             * @return a mutable reference to the underlying data
             * @throws scl::exceptions::UninitializedMemoryAccess
             */
            T& get() {
                if (!this->init) throw scl::exceptions::UninitializedMemoryAccess{};

                return *ptr();
            }

            /**
             * Access the underlying data
             * @return a mutable reference to the underlying data
             * @throws scl::exceptions::UninitializedMemoryAccess
             */
            T& operator*() { return this->get(); }

            /**
             * Get a pointer to the underlying data
             * @return a mutable pointer to the underlying data
             * @throws scl::exceptions::UninitializedMemoryAccess
             */
            T* operator->() { return &(this->get()); }

            /**
             * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the
             * storage)
             * @return an immutable reference to the underlying value
             */
            const T& get() const {
                if (!this->init) throw scl::exceptions::UninitializedMemoryAccess{};

                return *reinterpret_cast<const T*>(&this->storage);
            }

            /**
             * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the
             * storage)
             * @return an immutable reference to the underlying value
             */
            const T& operator*() const { return this->get(); }

            /**
             * Get a pointer to the value from a constant RawStorage (e.g. w/ a constant class that
             * uses the storage)
             * @return an immutable pointer to the underlying value
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
    }  // namespace utils
}  // namespace scl