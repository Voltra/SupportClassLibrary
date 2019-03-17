#pragma once

#include <functional>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/type_mod.h>

namespace scl{
	namespace tools{
		namespace meta{
			template <class>
			struct function_traits;

			template <class R, class... Args>
			struct function_traits<std::function<R(Args...)>>{
				constexpr static size_t arity = sizeof...(Args);

				using return_type = R;

				template <size_t i>
				using arg_t = typename std::tuple_element<i, std::tuple<Args...>>::type;
			};

			template <class T>
			using return_t = typename function_traits<T>::return_type;

			template <class T, size_t i>
			using arg_t = typename function_traits<T>::template arg_t<i>;



			/*
			 * Huge thanks to
			 * https://stackoverflow.com/A/27826081/7316365
			 */
			template <class>
			struct fn;

			/*template <class F>
			struct fn : public fn<decltype(&F::operator())>{
			};*/

			template <class R, class... Args>
			struct fn<R(*)(Args...)> /*: public std::function<R(Args...)>*/{
				using fn_type = std::function<R(Args...)>;
			};

			template <class Class, class R, class... Args>
			struct fn<R(Class::*)(Args...)> /*: public std::function<R(const Class&, Args...)>*/{
				using fn_type = std::function<R(const Class&, Args...)>;
			};


			template <class F, class = enable_if_t<
				exists<decltype(&F::operator())>()
			>>
			typename META::fn<F>::fn_type as_fn(F f){
				return (typename META::fn<F>::fn_type){f};
			}

			template <class R, class... Args>
			typename META::fn<R(*)(Args...)>::fn_type as_fn(R(*f)(Args...)){
				return (typename META::fn<R(*)(Args...)>::fn_type){f};
			}

			template <class Class, class R, class... Args>
			typename META::fn<R(Class::*)(Args...)>::fn_type as_fn(R(Class::* f)(Args...) const){
//				return (typename META::fn<R(Class::*)(Args...)>::fn_type){f};
				return std::mem_fn(f);
			}
		}
	}
}