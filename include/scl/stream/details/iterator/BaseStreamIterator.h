#pragma once
#include <scl/stream/details/deprecation.h>
#include <scl/stream/details/iterator/StreamIterator.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				/**
				 * A class for iterators that start a stream chain
				 * @tparam T being the type of data manipulated
				 */
				template <class T>
				class BaseStreamIterator : public virtual StreamIterator<T>{
					public:
						/**
						 * @typedef iterator_type
						 * Type alias for the iterator type
						 */
						using iterator_type = StreamIterator<T>;

//						virtual BaseStreamIterator<T> clone() const = 0;
				};
			}
		}
	}
}