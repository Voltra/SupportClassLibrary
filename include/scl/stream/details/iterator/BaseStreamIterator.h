#pragma once

#include <scl/stream/details/iterator/StreamIterator.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				template <class T>
				class BaseStreamIterator : public virtual StreamIterator<T>{
					public:
						using iterator_type = StreamIterator<T>;

//						virtual BaseStreamIterator<T> clone() const = 0;
				};
			}
		}
	}
}