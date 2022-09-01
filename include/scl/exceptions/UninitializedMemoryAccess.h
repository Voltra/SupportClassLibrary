#pragma once
#include "../macros.h"

#if !SCL_NO_EXCEPTIONS
    #include "../meta/type_aliases/real_const.h"
    #include "./RuntimeException.h"

namespace scl {
    namespace exceptions {
        /**
         * Exception class used when trying to access uninitialized memory
         */
        struct UninitializedMemoryAccess : public RuntimeException {
                using RuntimeException::RuntimeException;

                UninitializedMemoryAccess()
                    : UninitializedMemoryAccess("Trying to access uninitialized memory") {}

                scl::meta::real_const_t<char*> type() const noexcept override {
                    return "scl::exceptions::UninitializedMemoryAccess";
                }
        };
    }  // namespace exceptions
}  // namespace scl
#endif