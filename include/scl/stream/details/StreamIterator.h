#pragma once
#include "../../meta/type_queries/stl.h"
#include "./StreamPayload.h"

namespace scl {
    namespace stream {
	namespace details {
	    template <class>
	    class StlAdapter;

	    /**
	     * Abstract base class for stream iterators defining the required behavior
	     * @tparam T being the type manipulated throughout the stream
	     */
	    template <class T>
	    class StreamIterator {
		    static_assert(!scl::meta::is_void<T>(), "Cannot make a StreamIterator<void>");

		public:
		    /**
		     * @typedef payload_type
		     * Type alias for the payload used
		     */
		    using payload_type = StreamPayload<T>;

		    /**
		     * @typedef value_type
		     * Type alias for the data type manipulated
		     */
		    using value_type = typename payload_type::value_type;

		    /**
		     * Determines whether or not this iterator can produce another value payload
		     * @return TRUE if it does, FALSE otherwise
		     */
		    virtual bool hasNext() const = 0;

		    /**
		     * Retrieve the next iterator payload
		     * @return the payload
		     */
		    virtual payload_type next() = 0;

		    /**
		     * Retrieve a stl-like iterator for this stream iterator
		     * @return an iterator for the current state of this stream iterator
		     */
		    virtual StlAdapter<T> begin() {
			return StlAdapter<T>{this};
		    }

		    /**
		     * Retrieve a stl-like iterator for the end of this stream iterator
		     * @return an iterator for the invalid/end state of this stream iterator
		     */
		    virtual StlAdapter<T> end() {
			return StlAdapter<T>{};
		    }
	    };
	}  // namespace details
    }      // namespace stream
}  // namespace scl