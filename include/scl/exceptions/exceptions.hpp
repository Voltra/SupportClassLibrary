#pragma once
#include "../macros.h"

#if !SCL_NO_EXCEPTIONS
namespace scl {
    /**
     * @namespace scl::exceptions
     */
    namespace exceptions {}
}  // namespace scl

    #include "./EmptyOptionalAccess.h"
    #include "./Exception.h"
    #include "./InvalidEitherAccess.h"
    #include "./RuntimeException.h"
    #include "./Throwable.h"
    #include "./UninitializedMemoryAccess.h"
#endif