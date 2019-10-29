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
#include <scl/utils/RawStorage.h>
#include <scl/tools/meta/defines_scl_to_string.h>
#include <iostream>

#include <scl/tools/iostream/nl.h>
#include <scl/tools/meta/conditional.h>

#include <scl/concepts/require.h>
#include <scl/concepts/Movable.h>

namespace scl{
	namespace utils{
		/**
		 * An empty class serving as the type of an empty Optional<T>
		 */
		struct None final{
			constexpr bool operator==(None) const{ return true; }
			constexpr bool operator!=(None) const{ return false; }
			constexpr bool operator<(None) const{ return false; }
			constexpr bool operator<=(None) const{ return true; }
			constexpr bool operator>(None) const{ return false; }
			constexpr bool operator>=(None) const{ return true; }
		};

		/**
		 * @var none
		 * A constant global variable of type None
		 */
		constexpr None none = None{};

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
			constexpr stringLiteral operator()(const None&) const{
				return "[none ; scl::utils::None]";
			}
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
				 * @var payload
				 * A raw storage to hold an instance of the object
				 */
				RawStorage<T> payload;

			public:
				using value_type = META::remove_cv_ref_t<T>;

			public:
				/**
				 * Construct an optional from a pointer
				 * @param ptr being the pointer to construct from
				 * @return an empty optional if ptr is null, an initialized pointer otherwise
				 */
				constexpr static Optional fromPointer(const T* ptr){
					return !ptr ? Optional{none} : Optional{*ptr};
				}

				constexpr static Optional fromPointer(std::nullptr_t){
					return none;
				}

				/**
				 * Construct an optional inplace
				 * @tparam Args being the types of the arguments for the constructor
				 * @param args being the arguments for the constructor
				 * @return an initialized optional
				 */
				template <class... Args>
				static Optional inplace(Args&&... args){
					Optional ret = none;
					value_type value{std::forward<Args>(args)...};
					ret = std::move(value);
					return std::move(ret);
				}

				/**
				 * Construct an optional from a reference to an object
				 * @param ref being the reference to construct from
				 * @return an initialized optional
				 */
				static Optional ref(const value_type& ref){
					Optional ret = none;
					ret.payload.construct(ref);
					return ret;
				}

				/**
				 * Default construct (no value)
				 */
				Optional() : payload{} {
				}

				/**
				 * Determines whether or not this Optional<T> is empty
				 * @return TRUE if there's a value, FALSE otherwise
				 */
				bool hasValue() const{
					return this->payload.hasValue();
				}

				/**
				 * Retrieves the value stored in this Optional<T>
				 * @return a const& to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				const value_type& get() const{
					if(!this->hasValue())
						throw exceptions::EmptyOptionalAccess{};

					return this->payload.get();
				}

				/**
				 * Creates a non empty optional with the given value (copy)
				 * @param value being the value to assign from
				 */
				Optional(const value_type& value){
					this->payload.construct(value);
				}

				/**
				 * Assign a value to this optional
				 * @param value being the value taken by the optional
				 * @return a reference to this Optional
				 */
				Optional& operator=(const value_type& value){
					this->payload.construct(value);
					return *this;
				}

				/**
				 * Copy constructor
				 * @param o being the Optional to copy from
				 */
				Optional(const Optional& o){
					if(o.hasValue())
						this->payload.construct(o.get());
				};

				/**
				 * Copy assignment operator
				 * @param rhs being the Optional to assign from
				 * @return a reference to this Optional<T>
				 */
				Optional& operator=(const Optional& rhs){
					if(rhs.hasValue())
						this->payload.construct(rhs.get());

					return *this;
				};

				/**
				 * Automatic conversion operator
				 * @return a const reference to the underlying value
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				operator const value_type&() const{ return this->get(); }

				/**
				 * Move constructor
				 * @param rhs being the Optional to construct from
				 *
				 * @warning moved-from Optional are in empty state
				 * (i.e. equivalent to one constructed from none)
				 */
				Optional(Optional&& rhs){
					if(rhs.hasValue())
						this->payload = std::move(rhs.payload);
				};

				/**
				 * Move assignment operator
				 * @param rhs being the Optional to assign from
				 * @return a reference to this Optional<T>
				 *
				 * @warning moved-from Optional are in empty state
				 * (i.e. equivalent to one constructed from none)
				 */
				Optional& operator=(Optional&& rhs) noexcept{
					if(rhs.hasValue())
						this->payload = std::move(rhs.payload);
					return *this;
				};


			public:
				/**
				 * Instantiate an optional via an instance of None
				 * @param _ being an instance of None
				 */
				Optional(None _) : Optional(){
				}

				/**
				 * Assign from None
				 * @param _ being an instance of None
				 * @return a reference to this Optional
				 */
				Optional& operator=(None _){
					this->payload.reset();
					return *this;
				}

#define TPL template <class U, class = META::enable_if_t<\
	!META::is_same<META::decay_t<U>, META::decay_t<T>>()\
	&& !META::is_same<META::decay_t<U>, None>()\
	&& !META::is_same<META::decay_t<U>, Optional>()\
>>
				/**
				 * Implicit conversion copy constructor
				 * @tparam U being the type to implicitly convert from
				 * @param value being the value to construct from
				 */
				TPL
				Optional(const U& value){
					this->payload.construct(value);
				}

				/**
				 * Implicit conversion copy assignment
				 * @tparam U being the type to implicitly convert from
				 * @param value being the value to assign from
				 * @return a reference to this Optional
				 */
				TPL
				Optional& operator=(const U& value){
					this->payload.construct(value);
					return *this;
				}

				/**
				 * Implicit conversion move constructor
				 * @tparam U being the type to implicitly convert from
				 * @param value being the value to construct from
				 */
				TPL
				Optional(U&& value){
					this->payload.construct(std::forward<U&&>(value));
				}

				/**
				 * Implicit conversion move assignment
				 * @tparam U being the type to implicitly convert from
				 * @param value being the value to assign from
				 * @return a reference to this Optional
				 */
				TPL
				Optional& operator=(U&& value){
					this->payload.construct(std::forward<U&&>(value));
					return *this;
				}

#undef TPL

				/**
				 * A semantic alias for Optional<T>::get
				 * @return a const& to the stored value
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				const value_type& value() const{ return this->get(); }

				/**
				 * Get an immutable pointer to the contained value
				 * @return a realConst(T*) to the value
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				realConst(value_type*) ptr() const{ return &(this->get()); }

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
				const value_type& operator*() const{ return this->get(); }

				/**
				 * Get an immutable pointer to the stored value
				 * @return a realConst T* to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				realConst(value_type*) operator->() const{ return this->ptr(); }

				/**
				 * Retrieves the value if there's one or return the default value provided
				 * @param defaultValue being the value to return if there's no values
				 * @return a const reference to the stored value
				 */
				const value_type& orElse(const value_type& defaultValue) const{
					return this->hasValue() ? this->get() : defaultValue;
				}

				/**
				 * Calls a function if the value is present
				 * @tparam F being the function's type
				 * @param f being the function to call
				 * @return a reference to this optional (for chaining purposes)
				 */
				template <class F>
				const Optional& doIfPresent(F&& f) const{
					if(this->hasValue())
						f(this->get());

					return *this;
				}

				/**
				 * Alias for scl::utils::Optional::doIfPresent
				 */
				template <class F>
				const Optional& ifSome(F&& f) const{
					return this->doIfPresent(std::forward<F>(f));
				}

				/**
				 * Calls a function if there is no value
				 * @tparam F being the function's type
				 * @param f being the function to call
				 * @return a reference to this optional (for chaining purposes)
				 */
				template <class F>
				const Optional& doIfEmpty(F&& f) const{
					if(!this->hasValue())
						f();

					return *this;
				}

				/**
				 * Alias for scl::utils::Optional::doIfEmpty
				 */
				template <class F>
				const Optional& ifNone(F&& f) const{
					return this->doIfEmpty(std::forward<F>(f));
				}

				/**
				 * Tries to retrieve the value, throws the given exception if there's none
				 * @tparam E being the exception type
				 * @param e being the exception to throw if there's no value
				 * @return a const& to the value
				 */
				template <class E>
				const value_type& orThrow(const E& e) const{
					if(this->hasValue())
						return this->get();

					throw e;
				}

				/**
				 * Maps this Optional<T> to an Optional<U> via the provided mapper function (T -> U)
				 * @tparam U being the value type for the mapped optional
				 * @tparam F being the type of the mapper function (auto deduction)
				 * @param mapper being the mapper function to use to map values
				 * @return an Optional<U>
				 */
				template <class U, class F>
				Optional<U> map(F&& mapper) const{
					if(this->hasValue()){
						const value_type& _ = this->get();
						return mapper(_);
					}

					return none;
				}

				/**
				 * Alias for Optional::map
				 */
				template <class U, class F>
				Optional<U> mapTo(F&& mapper) const{ return this->map<U>(mapper); }

				/**
				 * Filters the value according to the given predicate
				 * @tparam F being the type of predicate (auto deduction)
				 * @param predicate being the predicate used to determine whether or not it should keep the value
				 * @return a new optional that might not contain the original value
				 */
				template <class F>
				Optional<T> filter(F predicate) const{
					if(this->hasValue()){
						const value_type& _ = this->get();
						return predicate(_) ? Optional<T>{_} : Optional<T>{};
					}

					return none;
				}

				/**
				 * Flat maps this optional to an optional of another type
				 * @tparam U being the value type of the mapped optional
				 * @tparam F being the mapper's type
				 * @param mapper being the mapper function
				 * @return the mapped optional
				 */
				template <class U, class F>
				Optional<U> flatMap(F&& mapper) const{
					return this->hasValue() ? mapper(this->get()) : none;
				}

				/**
				 * Alias for Optional::flatMap
				 */
				template <class U, class F>
				Optional<U> flatMapTo(F&& mapper) const{
					return this->flatMap<U>(std::forward<F>(mapper));
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

		template <class T>
		struct ToString<Optional<T>, META::enable_if_t<
			META::defines_scl_to_string<T>()
		>>{
			std::string operator()(const Optional<T>& opt){
				return opt.hasValue() ? asString(*opt) : asString(none);
			}
		};
	}
}