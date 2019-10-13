#pragma once

#include <string>
#include <scl/macros.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/tools/meta/is_same.h>

namespace scl{
	namespace utils{
		/**
		 * Class used to convert a type to a std::string
		 * @tparam T being the type of objects to convert to string
		 * @warning must define the call operator
		 * @example std::string operator()(const int& i){ return std::to_string(i); }
		 */
		template <class T, class=void>
		struct ToString;

		/**
		 * Specialization for types convertible to string
		 * @tparam T being the type of objects to convert to string
		 */
		template <class T>
		struct ToString<T, META::void_t<META::enable_if_t<
			META::is_same<T, char>()
			|| META::is_same<T, const char*>()
			|| META::is_same<T, std::string>()
		>>>{
			std::string operator()(const T& t) const{
				return std::string{t};
			}
		};

		/**
		 * Free function that allows string conversion
		 * @tparam T the type of object to convert to string
		 * @returns the string representation of the object
		 */
		template <class T>
		std::string toString(const T& obj){
			static_assert(
				META::exists<ToString<META::decay_t<T>>>(),
				"ToString<T> has not been defined"
			);
			return ToString<META::decay_t<T>>{}(obj);
		}
	}
}