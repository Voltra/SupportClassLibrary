#pragma once
#include "../../meta/meta.hpp"
#include "../Stream.h"
#include "../details/OpStreamIterator.h"

namespace scl {
    namespace stream {
        namespace operators {
            namespace details {
                template <class T, class U>
                using mapper_type = std::function<U(T&)>;

                /**
                 * Mapping operation
                 * @tparam T being the type to map from
                 * @tparam U being the type to map to
                 */
                template <class T, class U, class ParentIterator>
                class MapOperator final
                    : public scl::stream::details::OpStreamIterator<U, ParentIterator> {
                public:
                    using iterator_type = scl::stream::details::OpStreamIterator<U, ParentIterator>;
                    using value_type = typename iterator_type::value_type;
                    using payload_type = typename iterator_type::payload_type;

                    using parent_iterator_type = typename iterator_type::parent_iterator_type;
                    using parent_value_type = typename parent_iterator_type::value_type;
                    using parent_payload_type = typename parent_iterator_type::payload_type;
                    using parent_type = typename iterator_type::parent_type;

                    /**
                     * @typedef mapper_type
                     * Function type that maps the parent value type to the current value type
                     */
                    using mapper_type
                        = scl::stream::operators::details::mapper_type<parent_value_type,
                                                                       value_type>;

                    /**
                     * Construct the operator from the parent iterator and a mapper function
                     * @param p being the parent iterator
                     * @param mapper being the mapper function
                     */
                    MapOperator(parent_type p, mapper_type mapper)
                        : iterator_type{std::move(p)}, mapper{std::move(mapper)} {}

                    payload_type next() final {
                        auto next = this->parent().next();
                        auto&& alt = next.value();

                        return alt.hasValue() ? payload_type::withValue(this->mapper(*alt))
                                              : payload_type::withoutValue();
                    }

                protected:
                    /**
                     * @var mapper
                     * the function used to map values from the parent iterator
                     */
                    mapper_type mapper;
                };

                /**
                 * Tag type that allows operation piping for map operations
                 * @tparam T being the type to map from
                 * @tparam U being the type to map to
                 */
                template <class T, class U>
                struct map_operator_payload {
                    /**
                     * @typedef mapper_t
                     * The mapper function type
                     */
                    using mapper_t = mapper_type<T, U>;

                    /**
                     * @var mapper
                     * the mapper function
                     */
                    mapper_t mapper;
                };
            }  // namespace details

            /**
             * Map a stream
             * @tparam F being the type of the callable (deduced)
             * @tparam Fn being the function wrapper type (computed)
             * @tparam T being the type to map from (computed)
             * @tparam U being the type to map to (computed)
             * @param mapper being the callable used to map values
             * @return a toolbox tag for pipe operators
             */
            template <class F, class T = scl::meta::decay_t<scl::meta::arg_t<F, 0>>,
                      class U = scl::meta::decay_t<scl::meta::return_t<F>>>
            details::map_operator_payload<T, U> map(F&& mapper) {
                using mapper_t = typename details::map_operator_payload<T, U>::mapper_t;

                return {mapper_t{std::forward<F>(mapper)}};
            }

            /**
             * Map a stream by explicitly providing the type arguments
             * @tparam T being the type to map from
             * @tparam U being the type to map to
             * @param mapper being the mapper function
             * @return the toolbox tag for pipe operators
             */
            template <class T, class U = T>
            details::map_operator_payload<T, U> map_(
                typename details::map_operator_payload<T, U>::mapper_t mapper) {
                return {mapper};
            }

            /**
             * Pipe operator overload for map toolbox tags
             * @tparam T being the type to map from
             * @tparam U being the type to map to
             * @return The mapped stream
             */
            template <class T, class U, class It>
            auto operator|(Stream<T, It>&& lhs, details::map_operator_payload<T, U>&& rhs)
                -> SCL_RETURN(Stream<U, details::MapOperator<T, U, It>>{
                    details::MapOperator<T, U, It>{std::move(lhs.it()), rhs.mapper}})
        }  // namespace operators
    }      // namespace stream
}  // namespace scl