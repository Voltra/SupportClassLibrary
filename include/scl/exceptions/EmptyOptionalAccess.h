#pragma once
#include "../meta/meta.hpp"
#include "./Exception.h"

namespace scl {
    namespace exceptions {
        struct EmptyOptionalAccess : public Exception {
            using Exception::Exception;

            /**
             * Constructor that uses a default error message
             */
            EmptyOptionalAccess()
                : EmptyOptionalAccess("Tried to access the value of an empty Optional") {}

            scl::meta::real_const_t<char*> type() const noexcept override {
                return "scl::exceptions::EmptyOptionalAccess";
            }
        };
    }  // namespace exceptions
}  // namespace scl