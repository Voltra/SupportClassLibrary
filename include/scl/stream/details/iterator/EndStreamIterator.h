#pragma once

#include <scl/stream/details/iterator/StreamIterator.h>
#include <scl/stream/details/iterator/OpStreamIterator.h>
#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/stream/details/iterator/StlAdapter.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				/**
				 * A class for iterators used in end operations
				 * @tparam T being the type of data manipulated
				 * @tparam Ret being the type of the processed data
				 */
				template <class Ret, class T>
				class EndStreamIterator : public virtual StreamIterator<T>{
					public:
						/**
						 * @typedef iterator_type
						 * Type alias for the iterator type
						 */
						using iterator_type = StreamIterator<T>;

						/**
						 * @typedef result_type
						 * Type alias for the computed result
						 */
						using result_type = Ret;

						/**
						 * @typedef payload_type
						 * Type alias for the payload used
						 */
						using payload_type = typename iterator_type::payload_type;

						/**
						 * @typedef parent_iterator_type
						 * Type alias for the "parent" iterator
						 */
						using parent_iterator_type = BaseStreamIterator<T>;

						using parent_type = typename OpStreamIterator<T>::parent_type;

						/**
						 * Process the return value
						 * @return the processed value
						 */
						virtual result_type process() = 0;

						/**
						 * Getter for the parent iterator
						 * @return a reference to the parent iterator
						 */
						parent_type parent() const{ return this->parent_; }

						/**
						 * Construct from a parent iterator
						 * @param p being this iterator's parent
						 */
						explicit EndStreamIterator(parent_type p) : parent_{std::move(p)}{
						}

						/**
						 * @see scl::stream::details::iterator::StreamIterator::hasNext
						 */
						bool hasNext() const override{ return this->parent_->hasNext(); }

						/**
						 * @see scl::stream::details::iterator::StreamIterator::next
						 */
						payload_type next() override{ return std::move(this->parent_->next()); }

						/**
						 * @see scl::stream::details::iterator::StreamIterator::begin
						 */
						StlAdapter<T> begin() override{
							return this->parent_->begin();
						}

						/**
						 * @see scl::stream::details::iterator::StreamIterator::end
						 */
						StlAdapter<T> end() override{
							return this->parent_->end();
						}

					protected:
						/**
						 * @var parent_
						 * @brief This iterator's parent
						 */
						parent_type parent_;
				};
			}
		}
	}
}