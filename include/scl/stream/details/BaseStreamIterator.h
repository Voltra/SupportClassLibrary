#pragma once

namespace scl {
    namespace stream {
        namespace details {
            /**
             * A class for iterators that start a stream chain
             * @tparam T being the type of data manipulated
             */
            template <class T>
            class BaseStreamIterator : public virtual StreamIterator<T> {
            public:
                /**
                 * @typedef iterator_type
                 * Type alias for the iterator type
                 */
                using iterator_type = StreamIterator<T>;
            };
        }  // namespace details
    }      // namespace stream
}  // namespace scl