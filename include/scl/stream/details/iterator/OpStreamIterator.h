#pragma once

#include <scl/stream/details/iterator/BaseStreamIterator.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				template <class T, class ParentT = T>
				class OpStreamIterator : public virtual BaseStreamIterator<T>{
					public:
						using parent_iterator_type = BaseStreamIterator<ParentT>;

						parent_iterator_type& parent() const{ return this->parent_; };

						explicit OpStreamIterator(parent_iterator_type& parent) : parent_{parent}{
						}

						bool hasNext() const override{ return this->parent_.hasNext(); }

					protected:
						parent_iterator_type& parent_;
				};
			}
		}
	}
}