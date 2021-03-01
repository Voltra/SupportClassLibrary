#pragma once

#include <functional>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/type_mod.h>

namespace scl{
	namespace tools{
		namespace meta{
			/**
			 * Traits for callable types
			 */
			template <class>
			struct function_traits;

			/**
			 * Specialization for std::function
			 * @tparam R being the return type
			 * @tparam Args being the arguments' types
			 */
			template <class R, class... Args>
			struct function_traits<std::function<R(Args...)>>{
				/**
				 * @static arity
				 * The arity of the callable
				 */
				constexpr static size_t arity = sizeof...(Args);

				/**
				 * @typedef return_type
				 * The return type of the callable
				 */
				using return_type = R;

				/**
				 * @typedef arg_t
				 * @tparam i being the index of argument in the argument list (0-based index)
				 * The type of the i-th argument of the callable
				 */
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
			template <class, class>
			struct fn;

			template <class R, class... Args>
			struct fn<R(*)(Args...), void> /*: public std::function<R(Args...)>*/{
				using fn_type = std::function<R(Args...)>;
			};

			template <class Class, class R, class... Args>
			struct fn<R(Class::* const)(Args...), void> /*: public std::function<R(const Class&, Args...)>*/{
				using fn_type = std::function<R(const Class&, Args...)>;
			};

			template <class Class, class R, class... Args>
			struct fn<R(Class::* const)(Args...), std::nullptr_t>{
				using fn_type = std::function<R(Args...)>;
			};

			template <class F>
			struct fn<F, /*META::enable_if_t<
				META::exists<*/decltype(&F::operator())/*>()
			>*/> : public fn<decltype(&F::operator()), std::nullptr_t>{
				using fn_type = typename fn<decltype(&F::operator()), std::nullptr_t>::fn_type;
			};

			template <class F>
			using fn_t = fn<F, void>;

			template <class F>
			using std_fn_type = typename fn_t<F>::fn_type;

			template <class R, class... Args>
			std_fn_type<R(*)(Args...)> as_fn(R(*f)(Args...)){
				return std_fn_type<R(*)(Args...)>{f};
			}

			template <class Class, class R, class... Args>
			std_fn_type<R(Class::* const)(Args...)> as_fn(R(Class::* const f)(Args...) const){
//				return (typename META::fn<R(Class::*)(Args...)>::fn_type){f};
				return std::mem_fn(f);
			}

			template <class F>
			typename fn<F, void>::fn_type as_fn(F f){
				return (typename fn<F, void>::fn_type){f};
			}
		}
	}
}