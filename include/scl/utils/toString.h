#pragma once

#include <string>
#include "../macros.h"
#include "../tools/meta/enable_if.h"
#include "../tools/meta/exists.h"
#include "../tools/meta/type_mod.h"
#include "../tools/meta/is_convertible.h"

namespace scl{
	namespace utils{
		/**
		 * Class used to convert a type to a std::string
		 * @tparam T
		 * @warning must define the call operator
		 * @example std::string operator()(const int& i){ return std::to_string(i); }
		 */
		template <class T, class=void>
		struct ToString;

		/**
		 * Specialization for types convertible to string
		 */
		template <class T>
		struct ToString<T, scl::tools::meta::enable_if_t<
			META::is_convertible<T, char>()
			|| META::is_convertible<T, char*>()
			|| META::is_convertible<T, std::string>()
		>>{
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
				META::exists<ToString<T>>(),
				"ToString<T> has not been defined"
			);
			return ToString<META::decay_t<T>>{}(obj);
		}
	}
}