#pragma once

#include <scl/macros.h>
#include <scl/exceptions/EmptyOptionalAccess.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/is_same.h>
#include <scl/tools/meta/is_instance.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/type_check.h>
#include <scl/tools/meta/type_query.h>
#include <scl/utils/toString.h>
#include <iostream>

//TODO: Make Optional compatible w/ non trivially copyable types

namespace scl{
	namespace utils{
		/**
		 * An empty class serving as the type of an empty Optional<T>
		 */
		struct None final{
			bool operator==(None) const{ return true; }
			bool operator!=(None) const{ return false; }
			bool operator<(None) const{ return false; }
			bool operator<=(None) const{ return true; }
			bool operator>(None) const{ return false; }
			bool operator>=(None) const{ return true; }
		};

		/**
		 * @var none
		 * A constant global variable of type None
		 */
		const None none = None{};

		/**
		 * @class ToString<None>
		 * A specialization of ToString for None type
		 */
		template <>
		struct ToString<None>{
			/**
			 * String representation of a None object
			 * @return String representation of a None object
			 */
			constexpr stringLiteral operator()(const None&) const{ return "[none ; scl::utils::None]"; }
		};

		/**
		 * A class that allows the use of optional types (might be there)
		 * @tparam T being the value type that is optional
		 * @warning Optional defines move and/or copy semantics only if the
		 * underlying type does as well
		 */
		template <class T>
		class Optional{
			protected:
				/**
				 * @var valueFlag determines whether or not there's a value inside this Optional<T>
				 */
				bool valueFlag = false;

				/**
				 * @var payload
				 * A scl::tools::meta::aligned_storage_t of the correct size and alignment
				 * to maintain an instance of the object
				 */
				META::aligned_storage_t<sizeof(T), alignof(T)> payload = {};

			public:
				/**
				 * Default constructor, no value and empty payload
				 */
				Optional() : valueFlag{false}, payload{} {
				}

				/**
				 * Instantiate an optional via an instance of None
				 */
				Optional(None) : Optional(){}

				/**
				 * Creates a non empty optional with the given value (move)
				 * @param value being the value to assign from
				 */
				 template <class = META::void_t<META::enable_if_t<
				 	META::is_movable<T>()
				 >>>
				Optional(T&& value) : valueFlag{true} {
//					this->payload.value = std::move(value);
					new(&payload)T{value};
				}

				/**
				 * Creates a non empty optional with the given value (copy)
				 * @param value being the value to assign from
				 */
				template <class = META::void_t<META::enable_if_t<
					META::is_copyable<T>()
				>>>
				Optional(const T& value) : valueFlag{true} {
//					this->payload.value = value;
					new(&payload)T{value};
				}

				/**
				 * Implicit conversion copy constructor
				 * @tparam U being the type to implicitly convert from
				 * @param value being the value to construct from
				 */
				template <class U, class = META::enable_if_t<
					!META::is_same<T, U>()
					&& !META::is_same<T, None>()
				>>
				Optional(const U& value) : valueFlag{true} {
//					this->payload.value = value;
					new(&payload)T{value};
				}

				/**
				 * Implicit conversion move constructor
				 * @tparam U being the type to implicitly convert from
				 * @param value being the value to construct from
				 */
				template <class U, class = META::enable_if_t<
					!META::is_same<T, META::decay_t<U>>()
					&& !META::is_same<None, META::decay_t<U>>()
					&& !META::is_same<Optional, META::decay_t<U>>()
				>>
				Optional(U&& value) : valueFlag{true} {
//					this->payload.value = std::move(value);
					new(&payload)T{value};
				}

				/**
				 * Copy constructor
				 */
				template <class = META::void_t<META::enable_if_t<
					META::is_copyable<T>()
				>>>
				Optional(const Optional& o) : valueFlag{o.valueFlag} {
					if(o.hasValue())
//						this->payload.value = o.payload.value;
						new(&payload)T{o.get()};
				};

				/**
				 * Move constructor
				 */
				template <class = META::void_t<META::enable_if_t<
					META::is_movable<META::decay_t<T>>()
				>>>
				Optional(Optional&& o) : valueFlag{o.valueFlag} {
					if(o.hasValue())
//						this->payload.value = std::move(o.payload.value);
						this->payload = std::move(o.payload);
				};

				/**
				 * Copy assignment operator
				 * @return a reference to this Optional<T>
				 */
				template <class = META::void_t<META::enable_if_t<
					META::is_copyable<T>()
				>>>
				Optional& operator=(const Optional& o){
					this->valueFlag = o.valueFlag;
					if(o.valueFlag)
//						this->payload.value = o.payload.value;
						new(&payload)T{o.get()};

					return *this;
				};

				/**
				 * Move assignment operator
				 * @return a reference to this Optional<T>
				 */
				template <class = META::void_t<META::enable_if_t<
					META::is_movable<T>()
				>>>
				Optional& operator=(Optional&& o) noexcept{
					this->valueFlag = o.valueFlag;
					if(o.valueFlag)
//						this->payload.value = std::move(o.payload.value);
						this->payload = std::move(o.payload);

					return *this;
				};

				/**
				 * Determines whether or not this Optional<T> is empty
				 * @return TRUE if there's a value, FALSE otherwise
				 */
				bool hasValue() const{ return this->valueFlag; }

				/**
				 * Retrieves the value stored in this Optional<T>
				 * @return a const& to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				const T& get() const{
					if(!this->hasValue())
						throw exceptions::EmptyOptionalAccess{};

					return *reinterpret_cast<const T*>(&payload);
				}

				/**
				 * Mutable accessor for the value stored
				 * @return a reference to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				T& get(){
					if(!this->hasValue())
						throw exceptions::EmptyOptionalAccess{};

					//reinterpret_cast is allowed here since we allocated for a T
					return *reinterpret_cast<T*>(&payload);
				}

				/**
				 * A semantic alias for Optional<T>::get
				 * @return a const& to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				const T& value() const{ return this->get(); }

				/**
				 * A semantic alias for Optional<T>::get
				 * @return a const& to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				T& value(){ return this->get(); }

				/**
				 * Automatic conversion operator
				 * @return a copy of the underlying value
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				 template <class = META::enable_if_t<
					 META::is_copyable<T>()
				 >>
				operator T() const{ return this->get(); }

				/**
				 * Get an immutable pointer to the contained value
				 * @return a realConst(T*) to the value
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				realConst(T*) ptr() const{ return &(this->get()); }

				/**
				 * Get a mutable pointer to the contained value
				 * @return a T* to the value
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				T* ptr(){ return &(this->get()); }

				/**
				 * Automatic bool conversion
				 * @return TRUE if there's a value, FALSE otherwise
				 */
				operator bool() const{ return this->hasValue(); }

				/**
				 * Access to the value
				 * @return a const& to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				const T& operator*() const{ return this->get(); }

				/**
				 * Mutable access to the value
				 * @return a reference to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				T& operator*(){ return this->get(); }

				/**
				 * Get an immutable pointer to the stored value
				 * @return a realConst T* to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				realConst(T*) operator->() const{ return this->ptr(); }

				/**
				 * Get a mutable pointer to the stored value
				 * @return a T* to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				T* operator->(){ return this->ptr(); }

				/**
				 * Retrieves the value if there's one or return the default value provided
				 * @param defaultValue being the value to return if there's no values
				 * @return a copy of the stored value
				 */
				T orElse(const T& defaultValue) const{
					try{
						return this->get();
					}catch(exceptions::EmptyOptionalAccess&){
						return defaultValue;
					}
				}

				/**
				 * Tries to retrieve the value, throws the given exception if there's none
				 * @tparam E being the exception type
				 * @param e being the exception to throw if there's no value
				 * @return a const& to the value
				 */
				template <class E>
				const T& orThrow(const E& e) const{
					try{
						return this->get();
					}catch(exceptions::EmptyOptionalAccess&){
						throw e;
					}
				}

				/**
				 * Maps this Optional<T> to an Optional<U> via the provided mapper function (T -> U)
				 * @tparam U being the value type for the mapped optional
				 * @tparam F being the type of the mapper function (auto deduction)
				 * @param mapper being the mapper function to use to map values
				 * @return an Optional<U>
				 */
				template <class U, class F>
				Optional<U> map(F mapper) const{
					try{
						const T& _ = this->get();
						return mapper(_);
					}catch(exceptions::EmptyOptionalAccess&){
						return none;
					}
				}

				/**
				 * Alias for Optional::map
				 */
				template <class U, class F>
				Optional<U> mapTo(F mapper) const{ return this->map<U>(mapper); }

				/**
				 * Filters the value accoding to the given predicate
				 * @tparam F being the type of predicate (auto deduction)
				 * @param predicate being the predicate used to determine whether or not it should keep the value
				 * @return a new optional that might not contain the original value
				 */
				template <class F>
				Optional<T> filter(F predicate) const{
					try {
						const T& _ = this->get();
						return predicate(_) ? Optional<T>{_} : Optional<T>{};
					}catch(exceptions::EmptyOptionalAccess&){
						return none;
					}
				}

			public:
				bool operator==(None) const{ return !this->hasValue(); }
				friend bool operator==(None, const Optional& o){ return o == none; }

				bool operator<(None) const{ return false; }
				friend bool operator<(None, const Optional&){ return true; }

				bool operator<=(None) const{ return (*this) == none; }
				friend bool operator<=(None, const Optional&){ return true; }

				bool operator>(None) const{ return true; }
				friend bool operator>(None, const Optional&){ return false; }

				bool operator>=(None) const{ return true; }
				friend bool operator>=(None, const Optional& o){ return o <= none; }

				bool operator!=(None) const{ return !((*this) == none); }
				friend bool operator!=(None, const Optional& o){ return o != none; }

#define SCL_TPL template <class U, class = META::enable_if_t<\
	!META::is_same<U, None>()\
	&& !META::is_same<U, Optional>()\
	&& !META::is_instance<scl::utils::Optional, U>()\
>>
				SCL_TPL
				bool operator==(const U& t) const{ return this->hasValue() && this->value() == t; }
				SCL_TPL
				bool operator!=(const U& t) const{ return !((*this) == t); }
				SCL_TPL
				bool operator<(const U& t) const{ return !this->hasValue() || this->value() < t; }
				SCL_TPL
				bool operator<=(const U& t) const{ return (*this) == t || (*this) < t; }
				SCL_TPL
				bool operator>(const U& t) const{ return !((*this) <= t); }
				SCL_TPL
				bool operator>=(const U& t) const{ return !((*this) < t); }

				SCL_TPL
				friend bool operator==(const U& t, const Optional& o){ return o == t; }
				SCL_TPL
				friend bool operator!=(const U& t, const Optional& o){ return o != t; }
				SCL_TPL
				friend bool operator<(const U& t, const Optional& o){ return o > t; }
				SCL_TPL
				friend bool operator<=(const U& t, const Optional& o){ return o >= t; }
				SCL_TPL
				friend bool operator>(const U& t, const Optional& o){ return o < t; }
				SCL_TPL
				friend bool operator>=(const U& t, const Optional& o){ return o <= t; }
#undef SCL_TPL

#define SCL_TPL template<class U/*, class = META::enable_if_t<!META::is_same<U,T>()>*/>
				SCL_TPL
				bool operator==(const Optional<U>& o) const{
					if(!this->hasValue())
						return !o.hasValue();

					return o.hasValue() && this->value() == o.value();
				}

				SCL_TPL
				bool operator!=(const Optional<U>& o) const{ return !((*this) == o); }

				SCL_TPL
				bool operator<(const Optional<U>& o) const{
					if(!this->hasValue())
						return o.hasValue();

					return o.hasValue() && this->value() < o.value();
				}

				SCL_TPL
				bool operator<=(const Optional<U>& o) const{ return (*this) == o || (*this) < o; }

				SCL_TPL
				bool operator>(const Optional<U>& o) const{ return o < (*this); }

				SCL_TPL
				bool operator>=(const Optional<U>& o) const{ return o <= (*this); }
#undef SCL_TPL
		};
	}
}