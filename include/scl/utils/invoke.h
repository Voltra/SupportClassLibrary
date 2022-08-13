#pragma once
#include <utility>

#include "../alias/functions.h"
#include "../meta/type_aliases/callables.h"
#include "../meta/type_queries/type_queries.hpp"

namespace scl {
    namespace utils {
        template <class Ret, class... Args>
        inline Ret invoke(scl::alias::signature_type<Ret, Args...> fn, Args&&... args) {
            return fn(std::forward<Args>(args)...);
        }


        template <class Ret, class... Args>
        inline Ret invoke(scl::alias::fn_ptr_type<Ret, Args...> fn, Args&&... args) {
            return fn(std::forward<Args>(args)...);
        }

        template <class Ret, class Class, class... Args>
        inline Ret invoke(scl::alias::mem_fn_type<Ret, Class, Args...> fn, Class& obj,
                          Args&&... args) {
            return (obj.*fn)(std::forward<Args>(args)...);
        }

        template <class Ret, class Class, class... Args>
        inline Ret invoke(scl::alias::const_mem_fn_type<Ret, Class, Args...> fn, const Class& obj,
                          Args&&... args) {
            return (obj.*fn)(std::forward<Args>(args)...);
        }

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
        inline Ret& invoke(scl::alias::mem_var_type<Ret, Class, Args...> fn, Class& obj) {
            return obj.*fn;
        }

        template <class Ret, class Class, class... Args>
        inline const Ret& invoke(scl::alias::const_mem_var_type<Ret, Class, Args...> fn,
                                 const Class& obj) {
            return obj.*fn;
        }

        template <class Ret, class Class>
        inline Ret& invoke(scl::alias::mem_var_type<Ret, Class, Args...> fn, Class* obj) {
            return obj->*fn;
        }

        template <class Ret, class Class, class... Args>
        inline const Ret& invoke(scl::alias::const_mem_fn_type<Ret, Class, Args...> fn,
                                 const Class* obj) {
            return obj->*fn;
        }

        template <class CallableObject, class... Args, class FnTrait = scl::meta::enable_if_t<
            scl::meta::exists<decltype(&CallableObject::operator())>()
        >, scl::meta::function_traits<CallableObject>>
        inline const FnTrait::return_type invoke(CallableObject&& callable, Args&&... args) {
            return std::forward<CallableObject>(callable)(std::forward<Args>(args)...);
        }
    }  // namespace utils
}  // namespace scl