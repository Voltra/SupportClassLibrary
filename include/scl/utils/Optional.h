#pragma once

#include "../exceptions/EmptyOptionalAccess.h"
#include "toString.h"
#include "../macros.h"
#include "../tools/meta/enable_if.h"
#include "../tools/meta/is_same.h"

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
		 * @var none being a constant global variable of type None
		 */
		const None none = None{};

		/**
		 * @class ToString<Non>
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
		 */
		template <class T>
		class Optional{
			protected:
				/**
				 * @var valueFlag determines whether or not there's a value inside this Optional<T>
				 */
				bool valueFlag = false;

				/**
				 * @union payload_t
				 * Sum type used to describe the Optional<T> (None + T)
				 */
				union payload_t{
					None none = {};
					T value;
				};

				/**
				 * @var payload being an instance of Optional<T>::payload_t
				 */
				payload_t payload{};

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
				 * Creates a non empty optional with the given value
				 * @param value being the value to assign
				 */
				Optional(T&& value) : valueFlag{true} {
					this->payload.value = std::forward<T>(value);
				}

				/**
				 * Copy constructor
				 */
				Optional(const Optional&) = default;

				/**
				 * Move constructor
				 */
				Optional(Optional&&) = default;

				/**
				 * Copy assignment operator
				 * @return a reference to this Optional<T>
				 */
				Optional& operator=(const Optional&) = default;

				/**
				 * Move assignment operator
				 * @return a reference to this Optional<T>
				 */
				Optional& operator=(Optional&&) = default;

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
						throw exceptions::EmptyOptionalAccess{"Tried to access the value of an empty Optional"};

					return this->payload.value;
				}

				/**
				 * A semantic alias for Optional<T>::get
				 * @return a const& to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				const T& value() const{ return this->get(); }

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
				 * Get an immutable pointer to the stored value
				 * @return a realConst T* to the value stored
				 * @throws scl::exceptions::EmptyOptionalAccess if there's no value
				 */
				realConst(T*) operator->() const{ return &(this->get()); }

				/**
				 * Retrieves the value if there's one or return the default value provided
				 * @param defaultValue being the value to return if there's no values
				 * @return a copy of the stored value
				 */
				T orElse(T&& defaultValue) const{
					try{
						return this->get();
					}catch(exceptions::EmptyOptionalAccess&){
						return std::forward<T>(defaultValue);
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
						return predicate(_) ? _ : none;
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

#define SCL_TPL template <class U, class = META::enable_if_t<!META::is_same<U, None>() && !META::is_same<U, Optional<T>>()>>
				SCL_TPL
				bool operator==(const U& t) const{ return this->hasValue() && this->value() == t; }
				SCL_TPL
				bool operator!=(const U& t) const{ return !((*this) == t); }
				SCL_TPL
				bool operator<(const U& t) const{ return !this->hasValue() || this->value() < t; }
				SCL_TPL
				bool operator<=(const U& t) const{ return (*this) == t || (*this) < t;  }
				SCL_TPL
				bool operator>(const U& t) const{ return !((*this) <= t); }
				SCL_TPL
				bool operator>=(const U& t) const{ return !((*this) < t); }

				SCL_TPL
				friend bool operator==(const U& t, const Optional& o){ return o == t; }
				SCL_TPL
				friend bool operator!=(const U& t, const Optional& o){ return o != t; }
				SCL_TPL
				friend bool operator<(const U& t, const Optional& o){ return !(o >= t); }
				SCL_TPL
				friend bool operator<=(const U& t, const Optional& o){ return !(o > t); }
				SCL_TPL
				friend bool operator>(const U& t, const Optional& o){ return !(o <= t); }
				SCL_TPL
				friend bool operator >=(const U& t, const Optional& o){ return !(o < t); }
#undef SCL_TPL

#define SCL_TPL template<class U, class = META::enable_if_t<!META::is_same<U,T>()>>
				SCL_TPL
				bool operator==(const Optional<U>& o) const{
					if(!this->hasValue())
						return !o.hasValue();

					return o.hasValue() && this->value() == o.value();
				}

				SCL_TPL
				bool operator!=(const Optional<U>& o) const{
					return !((*this) == o);
				}

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