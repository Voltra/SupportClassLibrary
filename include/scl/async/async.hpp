#pragma once

namespace scl{
	/**
	 * @namespace scl::async
	 * Base namespace for asynchronous APIs
	 */
	namespace async{}
}

#include <scl/async/with_traits.h>
#include <scl/async/with.h>
#include <scl/async/Mutexed.h>
#include <scl/async/AsyncQueue.h>
#include <scl/async/Channel.h>
#include <scl/async/Active.h>