#pragma once
#include <scl/stream/details/deprecation.h>
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

						using parent_type = std::shared_ptr<parent_iterator_type>;

						/**
						 * Getter for the parent iterator
						 * @return a reference to the parent iterator
						 */
						parent_type parent() const{ return this->parent_; };

						/**
						 * Construct from a parent iterator
						 * @param parent being this iterator's parent
						 */
						explicit OpStreamIterator(parent_type parent) : parent_{std::move(parent)}{
						}

						/**
						 * @see scl::stream::details::iterator::StreamIterator::hasNext
						 */
						bool hasNext() const override{ return this->parent_->hasNext(); }

					protected:
						/**
						 * @var parent_
						 * This iterator's parent
						 */
						parent_type parent_;
				};
			}
		}
	}
}