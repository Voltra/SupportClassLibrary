#pragma once

namespace scl{
	/**
	 * @namespace scl::exceptions
	 * The namespace englobing all exception types
	 */
	namespace exceptions{}
}

#include "Throwable.h"
#include "Exception.h"
#include "RuntimeException.h"

#include "InvalidArgument.h"
#include "OutOfBound.h"
#include "InvalidCast.h"
#include "InvalidAnyCast.h"
#include "EmptyOptionalAccess.h"
#include "InvalidEitherAccess.h"