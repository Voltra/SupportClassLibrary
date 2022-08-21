#pragma once
#include <cstddef>

#include "../type_queries/exists.h"
#include "./stl.h"

namespace scl {
    namespace meta {
        namespace details {
            /**
             * Traits for callable types
             */
            template <class FnType, class Sfinae = void>
            struct function_traits;

            template <class, class Sfinae = void>
            struct deduced_function_signature;

            template <class Ret, class... Args>
            struct deduced_function_signature<Ret(Args...)> {
                using type = Ret(Args...);
            };

            template <class Ret, class... Args>
            struct deduced_function_signature<Ret (*)(Args...)>
                : deduced_function_signature<Ret(Args...)> {};

            template <class Ret, class Class, class... Args>
            struct deduced_function_signature<Ret (Class::*)(Args...)>
                : deduced_function_signature<Ret(Args...)> {};

            template <class Ret, class Class, class... Args>
            struct deduced_function_signature<Ret (Class::*)(Args...) const>
                : deduced_function_signature<Ret(Args...)> {};

            template <class Ret, class Class>
            struct deduced_function_signature<Ret Class::*>
                : deduced_function_signature<Ret(void)> {};

            template <class Callable>
            struct deduced_function_signature<
                Callable,
                scl::meta::enable_if_t<scl::meta::exists<decltype(&Callable::operator())>()>>
                : deduced_function_signature<decltype(&Callable::operator())> {};
        }  // namespace details

        template <class Callable>
        using function_traits = typename details::function_traits<Callable>;

        template <class Callable>
        using return_t = typename function_traits<Callable>::return_type;

        template <class Callable, std::size_t I>
        using arg_t = typename function_traits<Callable>::template arg_t<I>;

        template <class Callable>
        using deduced_function_sig_t = typename details::deduced_function_signature<Callable>::type;

        namespace details {
            template <class Ret, class... Args>
            struct function_traits<Ret(Args...)> {
                using return_type = Ret;
                using args_type = std::tuple<Args...>;

                template <std::size_t I>
                using arg_t = scl::meta::tuple_element_t<I, args_type>;
            };

            template <class Ret, class... Args>
            struct function_traits<Ret (*)(Args...)> : function_traits<Ret(Args...)> {};

            template <class Ret, class Class, class... Args>
            struct function_traits<Ret (Class::*)(Args...)> : function_traits<Ret(Class, Args...)> {
            };

            template <class Ret, class Class, class... Args>
            struct function_traits<Ret (Class::*)(Args...) const> : function_traits<Ret(Class, Args...)> {
            };

            template <class Ret, class Class>
            struct function_traits<Ret Class::*> : function_traits<Ret(Class)> {};

            template <class Callable>
            struct function_traits<
                Callable,
                scl::meta::enable_if_t<scl::meta::exists<decltype(&Callable::operator())>()>>
                : function_traits<deduced_function_sig_t<Callable>> {};
        }  // namespace details
    }      // namespace meta
}  // namespace scl