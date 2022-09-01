#pragma once
#include <utility>

#include "../../utils/exchange.h"
#include "./BaseStreamIterator.h"

namespace scl {
    namespace stream {
	namespace details {
	    /**
	     * Iterator powering an intermediate operation
	     * @tparam T
	     * @tparam ParentT
	     */
	    template <class T, class ParentIterator>
	    class OpStreamIterator : public virtual BaseStreamIterator<T> {
		public:
		    /**
		     * @typedef parent_iterator_type
		     * Type alias for the "parent" iterator
		     */
		    using parent_iterator_type = ParentIterator;

		    using parent_type = parent_iterator_type;

		    /**
		     * Getter for the parent iterator
		     * @return a reference to the parent iterator
		     */
		    parent_type& parent() {
			return this->parent_;
		    };

		    /**
		     * Construct from a parent iterator
		     * @param parent being this iterator's parent
		     */
		    explicit OpStreamIterator(parent_type parent) : parent_{std::move(parent)} {}

		    OpStreamIterator(const OpStreamIterator&) = delete;
		    OpStreamIterator& operator=(const OpStreamIterator&) = delete;

		    OpStreamIterator(OpStreamIterator&& rhs) noexcept
			: parent_{std::move(rhs.parent_)} {}

		    OpStreamIterator& operator=(OpStreamIterator&& rhs) noexcept {
			this->parent_ = std::move(rhs.parent_);
			return *this;
		    }

		    /**
		     * @see scl::stream::details::iterator::StreamIterator::hasNext
		     */
		    bool hasNext() const override {
			return this->parent_.hasNext();
		    }

		protected:
		    /**
		     * @var parent_
		     * This iterator's parent
		     */
		    parent_type parent_;
	    };
	}  // namespace details
    }      // namespace stream
}  // namespace scl