#pragma once
#include "./with_traits.h"

namespace scl {
    namespace async {
	/**
	 * Execute a transaction in a guarded manner
	 * @tparam Resource being the guarded/guarding resource type
	 * @tparam F being the transaction type
	 * @param res being the guarded/guarding resource
	 * @param f being the transaction to execute
	 */
	template <class Resource, class F>
	auto with(Resource& resource, F&& f) -> decltype(std::forward<F>(f)()) {
	    with_traits<Resource> traits;
	    return traits(resource, std::forward<F>(f));
	}
    }  // namespace async
}  // namespace scl