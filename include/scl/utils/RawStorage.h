#pragma once
#include <scl/macros.h>
#include <scl/exceptions/UninitializedMemoryAccess.h>
#include <scl/tools/meta/type_query.h>

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
				 using storage_type = META::aligned_storage_t<sizeof(T), alignof(T)>;

				/**
				 * @var init
				 * Determines whether or not the object has been constructed
				 */
				bool init;

				/**
				 * @var storage
				 * The actual memory that can hold the variable
				 */
				storage_type storage;

				storage_type* rawPtr(){
					return &storage;
				}

				T* ptr(){
					return reinterpret_cast<T*>(this->rawPtr());
				}

			public:
				RawStorage() : init{false}, storage{} {
				}

				~RawStorage(){
					this->destructor();
				}

				RawStorage(RawStorage&& other){
					*this = std::move(other);
				}

				RawStorage& operator=(RawStorage&& other){
					this->init = other.init;
					this->constructor(std::move(other.get()));
					other.init = false; //set the correct move semantics
					return *this;
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
					this->destructor();
					new( rawPtr() )T{std::forward<Args>(args)...};
					this->init = true;
					return this->get();
				}

				/**
				 * Call the destructor on the allocated object
				 * @warning If non initialized, it is a no-op
				 */
				void destructor(){
					if(this->init) {
						this->ptr()->~T();
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

					return *ptr();
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
				 * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the storage)
				 * @return an immutable reference to the underlying value
				 */
				const T& get() const{
					if(!this->init)
						throw scl::exceptions::UninitializedMemoryAccess{};

					return *reinterpret_cast<const T*>(&this->storage);
				}

				/**
				 * Get the value from a constant RawStorage (e.g. w/ a constant class that uses the storage)
				 * @return an immutable reference to the underlying value
				 */
				const T& operator*() const{
					return this->get();
				}

				/**
				 * Get a pointer to the value from a constant RawStorage (e.g. w/ a constant class that uses the storage)
				 * @return an immutable pointer to the underlying value
				 */
				realConst(T*) operator->() const{
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