#pragma once
#include <utility>

#include "../alias/functions.h"
#include "../meta/type_aliases/callables.h"
#include "../meta/type_queries/type_queries.hpp"

namespace scl {
    namespace utils {
        namespace details {
            template <class F, class Sfinae = void>
            struct invoker;

            template <class Ret, class... Args>
            struct invoker<scl::alias::signature_type<Ret, Args...>> {
                scl::alias::signature_type<Ret, Args...> callable;

                template <class... A>
                inline constexpr Ret operator()(A&&... args) {
                    return callable(std::forward<A>(args)...);
                }
            };

            template <class Ret, class... Args>
            struct invoker<scl::alias::fn_ptr_type<Ret, Args...>> {
                scl::alias::fn_ptr_type<Ret, Args...> callable;

                template <class... A>
                inline constexpr Ret operator()(A&&... args) {
                    return callable(std::forward<A>(args)...);
                }
            };

            template <class Ret, class Class, class... Args>
            struct invoker<scl::alias::mem_fn_type<Ret, Class, Args...>> {
                scl::alias::mem_fn_type<Ret, Class, Args...> callable;

                template <class C, class... A>
                inline constexpr Ret operator()(C&& obj, A&&... args) {
                    return (std::forward<C>(obj).*callable)(std::forward<A>(args)...);
                }
            };

            template <class Ret, class Class, class... Args>
            struct invoker<scl::alias::mem_fn_type<Ret, Class, Args...>&> {
                scl::alias::mem_fn_type<Ret, Class, Args...>& callable;

                template <class C, class... A>
                inline constexpr Ret operator()(C&& obj, A&&... args) {
                    return (std::forward<C>(obj).*callable)(std::forward<A>(args)...);
                }
            };

            template <class Ret, class Class, class... Args>
            struct invoker<scl::alias::const_mem_fn_type<Ret, Class, Args...>> {
                scl::alias::const_mem_fn_type<Ret, Class, Args...> callable;

                template <class C, class... A>
                inline constexpr Ret operator()(C&& obj, A&&... args) {
                    return (std::forward<C>(obj).*callable)(std::forward<A>(args)...);
                }
            };

            template <class Ret, class Class, class... Args>
            struct invoker<scl::alias::const_mem_fn_type<Ret, Class, Args...>&> {
                scl::alias::const_mem_fn_type<Ret, Class, Args...>& callable;

                template <class C, class... A>
                inline constexpr Ret operator()(C&& obj, A&&... args) {
                    return (std::forward<C>(obj).*callable)(std::forward<A>(args)...);
                }
            };

            template <class Ret, class Class>
            struct invoker<scl::alias::mem_var_type<Ret, Class>> {
                scl::alias::mem_var_type<Ret, Class> callable;

                template <class C>
                inline constexpr Ret operator()(C&& obj) {
                    return std::forward<C>(obj).*callable;
                }
            };

            template <class Ret, class Class>
            struct invoker<scl::alias::mem_var_type<Ret, Class>&> {
                scl::alias::mem_var_type<Ret, Class>& callable;

                template <class C>
                inline constexpr Ret operator()(C&& obj) {
                    return std::forward<C>(obj).*callable;
                }
            };

            template <class Ret, class Class>
            struct invoker<scl::alias::const_mem_var_type<Ret, Class>> {
                scl::alias::const_mem_var_type<Ret, Class> callable;

                template <class C>
                inline constexpr Ret operator()(C&& obj) {
                    return std::forward<C>(obj).*callable;
                }
            };

            template <class Ret, class Class>
            struct invoker<scl::alias::const_mem_var_type<Ret, Class>&> {
                scl::alias::const_mem_var_type<Ret, Class>& callable;

                template <class C>
                inline constexpr Ret operator()(C&& obj) {
                    return std::forward<C>(obj).*callable;
                }
            };

            template <class Callable>
            struct invoker<Callable, scl::meta::enable_if_t<
                                         scl::meta::exists<decltype(&Callable::operator())>()>> {
                Callable callable;

                template <class... Args>
                inline constexpr auto operator()(Args&&... args)
                    -> SCL_RETURN(callable(std::forward<Args>(args)...))
            };

            template <class Callable>
            struct invoker<Callable&, scl::meta::enable_if_t<
                                         scl::meta::exists<decltype(&Callable::operator())>()>> {
                Callable& callable;

                template <class... Args>
                inline constexpr auto operator()(Args&&... args)
                    -> SCL_RETURN(callable(std::forward<Args>(args)...))
            };
        }  // namespace details

        template <class F, class... Args/*,
                  class = scl::meta::enable_if_t<scl::meta::exists<details::invoker<F>>()>*/>
        inline constexpr auto invoke(F&& fn, Args&&... args)
            -> SCL_RETURN(details::invoker<F>{std::forward<F>(fn)}(std::forward<Args>(args)...))

        template <class Ret, class Class, class... Args>
        inline Ret invoke(scl::alias::mem_fn_type<Ret, Class, Args...> fn, Class* obj,
                          Args&&... args) {
            return (obj->*fn)(std::forward<Args>(args)...);
        }

        template <class Ret, class Class, class... Args>
        inline Ret invoke(scl::alias::const_mem_fn_type<Ret, Class, Args...> fn, const Class* obj,
                          Args&&... args) {
            return (obj->*fn)(std::forward<Args>(args)...);
        }

        template <class Ret, class Class>
        inline Ret& invoke(scl::alias::mem_var_type<Ret, Class> fn, Class* obj) {
            return obj->*fn;
        }

        template <class Ret, class Class>
        inline const Ret& invoke(scl::alias::const_mem_var_type<Ret, Class> fn,
                                 const Class* obj) {
            return obj->*fn;
        }
    }  // namespace utils
}  // namespace scl