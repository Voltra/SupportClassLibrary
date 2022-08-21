#pragma once
#include "../../meta/type_aliases/stl.h"
#include "../../meta/type_queries/is_iterator.h"
#include "../../macros.h"
#include "../details/BaseStreamIterator.h"
#include "../Stream.h"

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
                class FromStlIterator final
                    : public scl::stream::details::BaseStreamIterator<iterator_value_type<It>> {
                public:
                    using value_type = iterator_value_type<It>;
                    using iterator_type = FromStlIterator;
                    using payload_type = typename scl::stream::details::BaseStreamIterator<value_type>::payload_type;

                protected:
                    /**
                     * @var begin
                     * the iterator to the first element
                     *
                     * @var end
                     * the iterator to the point after the last element
                     */
                    It begin, end;

                public:
                    /**
                     * Construct the iterator from STL-like iterators
                     * @param begin being the beginning of the range
                     * @param end being the end of the range
                     */
                    FromStlIterator(It begin, It end) : begin{begin}, end{end} {}

                    bool hasNext() const final { return begin != end; }

                    payload_type next() final {
                        if (!this->hasNext()) return payload_type::withoutValue();

                        value_type& value = *(begin++);
                        return payload_type::withValue(std::move(value));
                    }
                };
            }  // namespace details

            template <class It, class = scl::meta::enable_if_t<
                scl::meta::is_iterator<It>()
            >>
            auto streamFrom(It begin, It end) -> SCL_RETURN(
                Stream<typename details::FromStlIterator<It>::value_type, details::FromStlIterator<It>>{
                    details::FromStlIterator<It>{begin, end}
                }
            )
        }      // namespace creators
    }          // namespace stream
}  // namespace scl
