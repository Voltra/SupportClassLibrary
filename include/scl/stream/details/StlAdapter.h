#pragma once

namespace scl {
    namespace stream {
        namespace details {
            template <class>
            class StreamIterator;

            /**
             * An adapter for STL-like iterators from stream iterators
             * @tparam T being the type of value handled
             */
            template <class T>
            class StlAdapter {
                public:
                    /**
                     * @typedef iterator_type
                     * The type of stream iterator handled
                     */
                    using iterator_type = StreamIterator<T>;

                    /**
                     * @typedef payload_type
                     * The type of stream iterator payload
                     */
                    using payload_type = typename iterator_type::payload_type;

                    /**
                     * @typedef iterator_category
                     * The category of this iterator type
                     */
                    using iterator_category = std::forward_iterator_tag;

                    /**
                     * @typedef value_type
                     * The type of value provided by the iterators
                     */
                    using value_type = payload_type;

                    /**
                     * @typedef difference_type
                     * The type of the difference in size between iterators
                     */
                    using difference_type = std::ptrdiff_t;

                    /**
                     * @typedef pointer
                     * Pointer type for the value type
                     */
                    using pointer = value_type*;

                    /**
                     * @typedef reference
                     * Reference type for the value type
                     */
                    using reference = value_type&;

                protected:
                    /**
                     * @var it
                     * A pointer to the iterator handled by this adapter
                     * @warning this does not manage its lifetime, the pointer must outlive the
                     * adapter
                     */
                    iterator_type* it;

                    /**
                     * @var payload
                     * The current payload
                     */
                    payload_type payload;

                public:
                    /**
                     * Construct an adapter from an iterator
                     * @param streamIterator being the stream iterator to construct from (nullptr
                     * for end iterator)
                     */
                    explicit StlAdapter(iterator_type* streamIterator = nullptr)
                        : StlAdapter(streamIterator, payload_type::withoutValue()) {}

                    /**
                     * Construct an adapter from an iterator and a specific payload
                     * @param streamIterator being the stream iterator to construct from (nullptr
                     * for end iterator)
                     * @param payload being the initial payload
                     */
                    StlAdapter(iterator_type* streamIterator, payload_type payload)
                        : it{streamIterator}, payload{std::move(payload)} {
                        ++(*this);  // increment because stream iterators are lazier (start without
                                    // initial value)
                    }

                    /**
                     * Pre-increment operator
                     * @return a reference to this adapter
                     */
                    StlAdapter& operator++() {
                        if (it == nullptr) {
                            return *this;
                        }

                        payload
                            = it->hasNext() ? std::move(it->next()) : payload_type::withoutValue();

                        return *this;
                    }

                    /**
                     * Post-increment operator
                     * @return a copy of this adapter before increment
                     */
                    StlAdapter operator++(int) {
                        auto ret = *this;
                        ++(*this);
                        return ret;
                    }

                    /**
                     * Equality operator
                     * @param rhs being the adapter to compare to
                     * @return TRUE if equal, FALSE otherwise
                     *
                     * @warning adapters are considered equal only if they both have
                     * no value (i.e. they are both the end iterator)
                     */
                    bool operator==(StlAdapter& rhs) {
                        auto& lhs = *this;

                        return lhs.payload.isInvalid() && rhs.payload.isInvalid();
                    }

                    /**
                     * Difference operator
                     * @param rhs being the adapter to compare to
                     * @return TRUE if different, FALSE otherwise
                     *
                     * @see scl::stream::details::StlAdapter::operator==
                     */
                    bool operator!=(StlAdapter& rhs) {
                        auto& lhs = *this;
                        return !(lhs == rhs);
                    }

                    /**
                     * Dereference operator
                     * @return the current payload
                     */
                    payload_type&& operator*() { return std::move(payload); }
            };
        }  // namespace details
    }      // namespace stream
}  // namespace scl