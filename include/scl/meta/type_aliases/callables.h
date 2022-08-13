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
            template <class>
            struct function_traits;
        }  // namespace details

        template <class Callable>
        using function_traits = typename details::function_traits<Callable>;

        template <class Callable>
        using return_t = typename function_traits<Callable>::return_type;

        template <class Callable, std::size_t I>
        using arg_t = typename function_traits<Callable>::template arg_t<I>;

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

            template <class Ret, class Class>
            struct function_traits<Ret Class::*> : function_traits<Ret(Class)> {};

            template <class Callable>
            struct function_traits<scl::meta::enable_if_t<
                scl::meta::exists<decltype(&Callable::operator())>(), Callable>>
                : function_traits<&Callable::operator()> {};
        }  // namespace details
    }      // namespace meta
}  // namespace scl