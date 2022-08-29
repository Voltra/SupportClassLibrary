#pragma once
#include "../meta/meta.hpp"

namespace scl {
    namespace stream {
        /**
         * Class representing a stream of data
         * @tparam T being the type of data
         */
        template <class T, class StreamIteratorType>
        class Stream {
            static_assert(!scl::meta::is_void<T>(), "Cannot make a Stream<void>");

        public:
            /**
             * @typedef iterator_type
             * The type of iterator used by this stream
             */
            using iterator_type = StreamIteratorType;  // details::BaseStreamIterator<T>;

            /**
             * @typedefe value_type
             * The value type used by this stream
             */
            using value_type = typename iterator_type::value_type;

            /**
             * Construct a stream a raw pointer to an iterator
             * @param iterator being a pointer to an iterator
             */
            explicit Stream(iterator_type iterator) : iterator{std::move(iterator)} {}

            Stream(const Stream&) = delete;
            Stream& operator=(const Stream&) = delete;

            Stream(Stream&&) noexcept = default;
            Stream& operator=(Stream&&) noexcept = default;

            /**
             * Get a reference to the underlying iterator
             * @return the underlying iterator
             */
            iterator_type& it() { return this->iterator; }

        protected:
            /**
             * @var iterator
             * The iterator used to get values
             */
            iterator_type iterator;
        };
    }  // namespace stream
}  // namespace scl