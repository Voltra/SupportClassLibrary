#pragma once

#include <utility>
#include <memory>
#include <scl/exceptions/InvalidAnyCast.h>
#include <scl/concepts/NonCopyable.h>
#include <scl/concepts/Movable.h>

namespace scl{
	namespace utils{
		struct __any_base{
			using type = void;

			virtual void polymorphism() const = 0;
			//template <class U>
			//U as() const{}
		};

		template <class T>
		class __any__impl final : public __any_base{
			protected:
				T value;

			public:
				using type = T;

				void polymorphism() const final{}

				__any__impl(T&& value) : value{std::forward<T>(value)} {
				}

				template <class U>
				U as() const{
					try{
						return static_cast<U>(this->value);
					}catch(...){
						throw exceptions::InvalidAnyCast{"Tried to cast Any to an unsupported type"};
					}
				}
		};

		/**
		 * Class that can hold any value type (and change value type mid lifetime)
		 */
		class Any{
			protected:
				/**
				 * @var impl
				 * A PIMPL
				 */
				std::unique_ptr<__any_base> impl;

				/**
				 * @var ti
				 * The type_info of the current value type
				 */
				const std::type_info* ti;

			public:
				/**
				 * Construct an Any with a value
				 * @tparam T being the given value type
				 * @param value being the value to construct from
				 */
				template <class T>
				Any(T&& value) : impl{new __any__impl<T>(std::forward<T>(value))}, ti{&typeid(T)} {
				}

				Any(Any&&) = default;
				Any& operator=(Any&&) = default;

				Any() = delete;
				Any(const Any&) = delete;
				Any& operator=(const Any&) = delete;

				/**
				 * Assign a new value to this Any
				 * @tparam T being the new value type
				 * @param value being the new value
				 * @return a reference to this Any
				 */
				template <class T>
				Any& operator=(T&& value){
					this->impl.reset(new __any__impl<T>(std::forward<T>(value)));
					this->ti = &typeid(T);
					return *this;
				}

				/**
				 * Safely determines whether or not you can cast from the current
				 * value type to the one given as argument
				 * @tparam U being the desired cast type
				 * @return TRUE if it is the current value type, FALSE otherwise
				 */
				template <class U>
				bool canCastTo() const{
					return this->ti->hash_code() == typeid(U).hash_code();
				}

				/**
				 * Attempts to cast this Any
				 * @tparam U being the type to cast to
				 * @return the type safe value
				 * @throws scl::exceptions::InvalidAnyCast if if can't cast
				 */
				template <class U>
				U as() const{
					if(this->canCastTo<U>())
						return dynamic_cast<__any__impl<U>*>(impl.get())->template as<U>();

					throw exceptions::InvalidAnyCast{"Tried to cast Any to an unsupported type"};
				}
		};
	}
}

/*
assert_concept(
	scl::concepts::Movable<scl::utils::Any>{} && scl::concepts::NonCopyable<scl::utils::Any>{},
	"scl::utils::Any should be non copyable and movable."
);*/
