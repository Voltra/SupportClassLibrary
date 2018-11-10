#pragma once

#include "../macros.h"
#include "../tools/meta/exists.h"
#include "../tools/meta/is_same.h"
#include "SemiRegular.h"
#include "Invocable.h"

#define SCL_ASSERT_TYPE(type) static_asssert(META::exists<T::type>(), "CharTraits<T, CharT>: T::type should exist")
#define SCL_TYPE(type) typename T::type
#define SCL_char std::declval<CharT>()
#define SCL_rptr std::declval<CharT*>()
#define SCL_ptr std::declval<const CharT*>()
#define SCL_size std::declval<std::size_t>()
#define SCL_int std::declval<int>()

namespace scl{
	namespace concepts{
		/**
		 * CharTraits concept
		 * @tparam T the type to check against
		 * @tparam CharT the character type to use
		 */
		template <class T, class CharT>
		struct CharTraits{
			constexpr operator bool() const{
				SCL_ASSERT_TYPE(char_type);
				SCL_ASSERT_TYPE(int_type);
				SCL_ASSERT_TYPE(off_type);
				SCL_ASSERT_TYPE(pos_type);
				SCL_ASSERT_TYPE(state_type);

				static_assert(META::is_same<SCL_TYPE(char_type), CharT>(), "CharTraits<T, CharT>: T::char_type should be CharT");
				static_assert(SemiRegular<SCL_TYPE(state_type)>{}, "CharTraits<T, CharT>: T::state_type should be SemiRegular");
				static_assert(
					META::is_same<decltype(T::eq(SCL_char, SCL_char)), bool>(),
					"CharTraits<T, CharT>: The result of T::eq(CharT, Chart) should be a bool"
				);
				static_assert(
					META::is_same<decltype(T::lt(SCL_char, SCL_char)), bool>(),
					"CharTraits<T, CharT>: The result of T::lt(CharT, Chart) should be a bool"
				);
				static_assert(
					META::is_same<decltype(T::compare(SCL_ptr, SCL_ptr, SCL_size)), int>(),
					"CharTraits<T, CharT>: The result of T::compare(CharT, Chart) should be an int"
				);
				static_assert(
					META::is_same<decltype(T::length(SCL_ptr)), std::size_t>(),
					"CharTraits<T, CharT>: The result of T::length(CharT, Chart) should be an std::size_t"
				);
				static_assert(
					META::is_same<decltype(T::find(SCL_ptr, SCL_size, SCL_char)), const CharT*>(),
					"CharTraits<T, CharT>: The result of T::find(const CharT*, std::size_t, CharT) should be a const CharT*"
				);
				static_assert(
					META::is_same<decltype(T::move(SCL_rptr, SCL_ptr, SCL_size)), CharT*>(),
					"CharTraits<T, CharT>: The result of T::move(SCL_rptr, SCL_ptr, SCL_size) should be a CharT*"
				);
				static_assert(
					META::is_same<decltype(T::copy(SCL_rptr, SCL_ptr, SCL_size)), CharT*>(),
					"CharTraits<T, CharT>: The result of T::copy(SCL_rptr, SCL_ptr, SCL_size) should be a CharT*"
				);
				static_assert(
//					Invocable<decltype(T::assign), CharT, CharT>{},
					META::exists<decltype(T::assign(SCL_char, SCL_char))>(),
					"CharTraits<T, CharT>: There should be T::assign(CharT, CharT)"
				);
				static_assert(
//					Invocable<decltype(T::assign), CharT*, std::size_t, CharT>{},
					META::is_same<decltype(T::assign(SCL_rptr, SCL_size, SCL_char)), CharT*>(),
					"CharTraits<T, CharT>: T::assign(CharT*, std::size_t, CharT) should return a CharT*"
				);
				static_assert(
					META::is_same<decltype(T::not_eof(SCL_int)), int>(),
					"CharTraits<T, CharT>: T::not_eof(int) should return an int"
				);
				static_assert(
					META::is_same<decltype(T::to_char_type(SCL_int)), CharT>(),
					"CharTraits<T, CharT>: T::to_char_type(int) should return a CharT"
				);
				static_assert(
					META::is_same<decltype(T::to_int_type(SCL_char)), int>(),
					"CharTraits<T, CharT>: T::to_int_type(CharT) should return an int"
				);
				static_assert(
					META::is_same<decltype(T::eq_int_type(SCL_int, SCL_int)), bool>(),
					"CharTraits<T, CharT>: T::to_int_type(int, int) should return a bool"
				);
				static_assert(
					META::is_same<decltype(T::eof()), int>(),
					"CharTraits<T, CharT>: T::eof() should return an int"
				);
				return true;
			}
		};
	}
}

#undef SCL_ASSERT_TYPE
#undef SCL_TYPE
#undef SCL_char
#undef SCL_rptr
#undef SCL_ptr
#undef SCL_size
#undef SCL_int