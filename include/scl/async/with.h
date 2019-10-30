#pragma once
#include <scl/async/with_traits.h>

namespace scl{
	namespace async{
		/**
		 * Execute a transaction in a guarded manner
		 * @tparam Resource being the guarded/guarding resource type
		 * @tparam F being the transaction type
		 * @param res being the guarded/guarding resource
		 * @param f being the transaction to execute
		 */
		template <class Resource, class F>
		auto with(Resource&& res, F&& f) -> META::return_t<F> {
			with_traits<Resource> traits;
			return traits(std::forward<Resource>(res), std::forward<F>(f));
		}
	}
}