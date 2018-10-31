#pragma once

namespace scl{
	/**
	 * @namespace scl:exceptions
	 * The namespace englobing all exception types
	 */
	namespace exceptions{}
}

#include <scl/exceptions/Throwable.h>
#include <scl/exceptions/Exception.h>
#include <scl/exceptions/RuntimeException.h>

#include <scl/exceptions/InvalidArgument.h>
#include <scl/exceptions/OutOfBound.h>
#include <scl/exceptions/InvalidCast.h>
#include <scl/exceptions/InvalidAnyCast.h>
#include <scl/exceptions/EmptyOptionalAccess.h>