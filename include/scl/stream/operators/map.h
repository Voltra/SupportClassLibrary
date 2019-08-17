#pragma once

#include <scl/stream/details/iterator/OpStreamIterator.h>
#include <functional>
#include <memory>
#include <scl/tools/make/make.hpp>
#include <scl/tools/meta/meta.hpp>

namespace scl{
	namespace stream{
		namespace operators{
			/**
			 * Mapping operation
			 * @tparam T being the type to map from
			 * @tparam U being the type to map to
			 */
			template <class T, class U>
			class MapOperator : public scl::stream::details::iterator::OpStreamIterator<U, T>{
				public:
					using iterator_type = scl::stream::details::iterator::OpStreamIterator<U, T>;
					using value_type = typename iterator_type::value_type;
					using payload_type = typename iterator_type::payload_type;

					using parent_iterator_type = typename iterator_type::parent_iterator_type;
					using parent_value_type = typename parent_iterator_type::value_type;
					using parent_payload_type = typename parent_iterator_type::payload_type;

					/**
					 * @typedef mapper_type
					 * Function type that maps the parent value type to the current value type
					 */
					using mapper_type = std::function<value_type(const parent_value_type&)>;

					/**
					 * Construct the operator from the parent iterator and a mapper function
					 * @param p being the parent iterator
					 * @param mapper being the mapper function
					 */
					MapOperator(parent_iterator_type& p, mapper_type mapper) : iterator_type{p}, mapper{mapper} {
					}

					payload_type next() override{
						/*if(!this->parent().hasNext())
							return payload_type::withoutValue();*/

						const auto& alt = this->parent().next().value();
						return payload_type{[&]{ return alt.template mapTo<value_type>(this->mapper); }};
					}

				protected:
					/**
					 * @var mapper
					 * the function used to map values from the parent iterator
					 */
					mapper_type mapper;
			};

			namespace details{
				/**
				 * Tag type that allows operation piping for map operations
				 * @tparam T being the type to map from
				 * @tparam U being the type to map to
				 */
				template <class T, class U>
				struct map_toolbox{
					/**
					 * @typedef mapper_t
					 * The mapper funtion type
					 */
					using mapper_t = typename MapOperator<T, U>::mapper_type;

					/**
					 * @var mapper
					 * the mapper function
					 */
					mapper_t mapper;
				};
			}


			/**
			 * Map a stream
			 * @tparam F being the type of the callable (deduced)
			 * @tparam Fn being the function wrapper type (computed)
			 * @tparam T being the type to map from (computed)
			 * @tparam U being the type to map to (computed)
			 * @param mapper being the callable used to map values
			 * @return a toolbox tag for pipe operators
			 */
			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::decay_t<META::arg_t<Fn, 0>>,
				class U = META::decay_t<META::return_t<Fn>>
			>
			details::map_toolbox<T, U> map(F&& mapper){
				return {META::as_fn(std::forward<F>(mapper))};
			}

			/**
			 * Map a stream by explicitly providing the type arguments
			 * @tparam T being the type to map from
			 * @tparam U being the type to map to
			 * @param mapper being the mapper function
			 * @return the toolbox tag for pipe operators
			 */
			template<class T, class U = T>
			details::map_toolbox<T, U> map_(typename details::map_toolbox<T, U>::mapper_t mapper){
				return {mapper};
			}

			/**
			 * Pipe operator overload for map toolbox tags
			 * @tparam T being the type to map from
			 * @tparam U being the type to map to
			 * @return The mapped stream
			 */
			template <class T, class U>
			Stream<U> operator|(const Stream<T>& lhs, const details::map_toolbox<T, U>& rhs){
				using namespace scl::tools;
				return Stream<U>{
					make::ptr<MapOperator<T, U>>(lhs.it(), rhs.mapper)
				};
			}
		}
	}
}