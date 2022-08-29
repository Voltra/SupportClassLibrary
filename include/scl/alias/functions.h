#pragma once
#include <functional>

namespace scl {
    namespace alias {
        template <class Ret, class... Args>
        using signature_type = Ret(Args...);

        template <class Ret, class... Args>
        using fn_ptr_type = Ret (*)(Args...);

        template <class Ret, class Class, class... Args>
        using mem_fn_type = Ret (Class::*)(Args...);

        template <class Ret, class Class, class... Args>
        using const_mem_fn_type = Ret (Class::*)(Args...) const;

        template <class Ret, class Class>
        using mem_var_type = Ret Class::*;

        template <class Ret, class Class>
        using const_mem_var_type = Ret Class::*const;

        template <class Ret, class... Args>
        using std_fn_type = std::function<Ret(Args...)>;
    }  // namespace alias
}  // namespace scl