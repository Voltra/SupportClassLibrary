#pragma once
#include <scl/exceptions/UninitializedMemoryAccess.h>

namespace scl{
	namespace utils{
		/**
		 * Class that handles raw storage (and manual memory management) for a variable type
		 * @tparam T being the variable to allocate storage for
		 * @warning Before using this, you must be certain this is what you're looking for
		 */
		template <class T>
		class RawStorage{
			protected:
				/**
				 * Union that stores the data type uninitialized
				 */
				union storage_type{
					storage_type(){}
					~storage_type(){}

					T value;
				};

				/**
				 * @var init
				 * Determines whether or not the object has been constructed
				 */
				bool init = false;

				/**
				 * @var storage
				 * The actual memory that can hold the variable
				 */
				storage_type storage;

			public:
				RawStorage() = default;
				~RawStorage(){
					this->destructor();
				}

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
				T& constructor(Args&&... args){
					this->ensureEmpty();
					new (&this->storage.value) T{std::forward<Args>(args)...};
					this->init = true;
					return this->storage.value;
				}

				/**
				 * Call the destructor on the allocated object
				 * @warning If non initialized, it is a no-op
				 */
				void destructor(){
					if(this->init) {
						this->storage.value.~T();
						this->init = false;
					}
				}

				/**
				 * Alias for RawStorage::constructor
				 */
				template <class... Args>
				T& construct(Args&&... args){
					return this->constructor(std::forward<Args&&>(args)...);
				}

				/**
				 * Alias for RawStorage::destructor
				 */
				void destroy(){
					this->destructor();
				}

				/**
				 * Access the underlying data
				 * @return a mutable reference to the underlying data
				 * @throws scl::exceptions::UninitializedMemoryAccess
				 */
				T& get(){
					if(!this->init)
						throw scl::exceptions::UninitializedMemoryAccess{};

					return this->storage.value;
				}

				/**
				 * Access the underlying data
				 * @return a mutable reference to the underlying data
				 * @throws scl::exceptions::UninitializedMemoryAccess
				 */
				T& operator*(){
					return this->get();
				}

				/**
				 * Get a pointer to the underlying data
				 * @return a mutable pointer to the underlying data
				 * @throws scl::exceptions::UninitializedMemoryAccess
				 */
				T* operator->(){
					return &(this->get());
				}

				/**
				 * Determine whether or not the storage holds a value
				 * @return TRUE if it does, FALSE otherwise
				 */
				bool hasValue() const{
					return this->init;
				}

				/**
				 * Implicit conversion to bool
				 * @return TRUE if the storage holds a value, FALSE otherwise
				 */
				operator bool() const{
					return this->hasValue();
				}
		};
	}
}