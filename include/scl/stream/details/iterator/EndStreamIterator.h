#pragma once

#include <scl/stream/details/iterator/StreamIterator.h>
#include <scl/stream/details/iterator/BaseStreamIterator.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				template <class Ret, class T>
				class EndStreamIterator : public virtual StreamIterator<T>{
					public:
						using iterator_type = StreamIterator<T>;
						using result_type = Ret;
						using payload_type = typename iterator_type::payload_type;
						using parent_iterator_type = BaseStreamIterator<T>;

						virtual result_type process() = 0;

						parent_iterator_type& parent() const{ return this->parent_; }

						explicit EndStreamIterator(parent_iterator_type& p) : parent_{p}{
						}

						bool hasNext() const override{ return this->parent_.hasNext(); }
						payload_type next() override{ return this->parent_.next(); }

					protected:
						parent_iterator_type& parent_;
				};
			}
		}
	}
}