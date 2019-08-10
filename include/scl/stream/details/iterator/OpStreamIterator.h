#pragma once

#include <scl/stream/details/iterator/BaseStreamIterator.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				/**
				 * Iterator powering an intermediate operation
				 * @tparam T
				 * @tparam ParentT
				 */
				template <class T, class ParentT = T>
				class OpStreamIterator : public virtual BaseStreamIterator<T>{
					public:
						/**
						 * @typedef parent_iterator_type
						 * Type alias for the "parent" iterator
						 */
						using parent_iterator_type = BaseStreamIterator<ParentT>;

						/**
						 * Getter for the parent iterator
						 * @return a reference to the parent iterator
						 */
						parent_iterator_type& parent() const{ return this->parent_; };

						/**
						 * Construct from a parent iterator
						 * @param p being this iterator's parent
						 */
						explicit OpStreamIterator(parent_iterator_type& parent) : parent_{parent}{
						}

						/**
						 * @see scl::stream::details::iterator::StreamIterator::hasNext
						 */
						bool hasNext() const override{ return this->parent_.hasNext(); }

					protected:
						/**
						 * @var parent_
						 * This iterator's parent
						 */
						parent_iterator_type& parent_;
				};
			}
		}
	}
}