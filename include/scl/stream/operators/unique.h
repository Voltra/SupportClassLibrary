#pragma once
#include <scl/stream/operators/uniqueBy.h>

namespace scl{
	namespace stream{
		namespace operators{
			namespace details{
				/**
				 * Toolbox tag for unique operations
				 */
				struct unique_toolbox{};
			}

			/**
			 * Remove duplicates from a stream
			 * @return a toolbox tag for pipe operator
			 */
			details::unique_toolbox unique(){
				return {};
			}

			/**
			 * Pipe operator overload for unique operations
			 * @tparam T being the value type
			 * @tparam U being the key type
			 * @param lhs being the stream to remove duplicates from
			 * @param rhs being the unique toolbox tag
			 */
			template <class T, class U>
			Stream<T> operator|(const Stream<T>& lhs, details::unique_toolbox rhs){
				return lhs | uniqueBy_<T, T>([](const T& t) -> const T&{
					return t;
				});
			}
		}
	};
}