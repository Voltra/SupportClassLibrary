#pragma once
#include "../../macros.h"
#include "../../meta/type_aliases/stl.h"
#include "../../meta/type_queries/is_iterator.h"
#include "../Stream.h"
#include "../details/BaseStreamIterator.h"

namespace scl {
    namespace stream {
        namespace creators {
            namespace details {
                /**
                 * @typedef iterator_value_type
                 * A type alias for the value type from the iterator traits
                 */
                template <class It>
                using iterator_value_type = typename std::iterator_traits<It>::value_type;

                /**
                 * Creator for an iterator-based range
                 * @tparam It being the type of the iterators
                 */
                template <class It>
                class FromIterators final : public virtual scl::stream::details::BaseStreamIterator<
                                                iterator_value_type<It>> {
                    public:
                        using value_type = iterator_value_type<It>;
                        using iterator_type = FromIterators;
                        using payload_type = typename scl::stream::details::BaseStreamIterator<
                            value_type>::payload_type;

                    protected:
                        /**
                         * @var begin
                         * the iterator to the first element
                         *
                         * @var end
                         * the iterator to the point after the last element
                         */
                        It begin_, end_;

                    public:
                        /**
                         * Construct the iterator from STL-like iterators
                         * @param begin being the beginning of the range
                         * @param end being the end of the range
                         */
                        FromIterators(It begin, It end) : begin_{begin}, end_{end} {}

                        bool hasNext() const final { return begin_ != end_; }

                        payload_type next() final {
                            if (!this->hasNext())
                                return payload_type::withoutValue();

                            value_type& value = *(begin_++);
                            return payload_type::withValue(std::move(value));
                        }
                };
            }  // namespace details

            template <class It, class = scl::meta::enable_if_t<scl::meta::is_iterator<It>()>>
            auto streamFrom(It begin, It end) -> SCL_RETURN(
                Stream<typename details::FromIterators<It>::value_type, details::FromIterators<It>>{
                    details::FromIterators<It>{begin, end}})
        }  // namespace creators
    }      // namespace stream
}  // namespace scl
