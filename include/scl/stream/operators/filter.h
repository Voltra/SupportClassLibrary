#pragma once

#include <scl/stream/details/iterator/OpStreamIterator.h>
#include <functional>
#include <memory>
#include <scl/tools/make/make.hpp>
#include <scl/tools/meta/meta.hpp>
#include <scl/stream/Stream.h>

namespace scl{
	namespace stream{
		namespace operators{
			namespace details{/**
			 * Filter operation
			 * @tparam T being the type of values to filter
			 */
				template <class T>
				class FilterOperator : public scl::stream::details::iterator::OpStreamIterator<T>{
					public:
						using iterator_type = scl::stream::details::iterator::OpStreamIterator<T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;
						using parent_iterator_type = typename iterator_type::parent_iterator_type;
						using parent_type = typename iterator_type::parent_type;

						/**
						 * @typedef predicate_type
						 * The function type used to determine whether to keep the value or not
						 */
						using predicate_type = std::function<bool(const value_type&)>;

						/**
						 * Construct a filter operation from a parent iterator and a predicate
						 * @param p being this iterator's parent
						 * @param pred being the predicate to fulfill
						 */
						FilterOperator(parent_type p, predicate_type pred) : iterator_type{std::move(p)}, pred{pred} {
						}

						payload_type next() override{
							while(this->parent()->hasNext()){
								const auto& alt = this->parent()->next().value();
								if(alt.hasValue()){
									const auto& value = *alt;
									if(this->pred(value))
										return payload_type::withValue(value);
								}
							}

							return payload_type::withoutValue();
						}
					protected:
						/**
						 * @var pred
						 * The predicate
						 */
						predicate_type pred;
				};

				/**
				 * Tag type for filter operations
				 * @tparam T being the value type
				 */
				template <class T>
				struct filter_toolbox{
					/**
					 * @typedef pred_t
					 * The predicate type
					 */
					using pred_t = typename FilterOperator<T>::predicate_type;

					/**
					 * @var pred
					 * the predicate
					 */
					pred_t pred;
				};
			}

			/**
			 * Filter a stream
			 * @tparam F being the type of the callable (deduced)
			 * @tparam Fn being the function wrapper type (computed)
			 * @tparam T being the value type (computed)
			 * @param predicate being the predicate to use
			 * @return a filter toolbox tag for pipe operator
			 */
			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::decay_t<META::arg_t<Fn, 0>>
			>
			details::filter_toolbox<T> filter(F&& predicate){
				return {META::as_fn(predicate)};
			}

			/**
			 * Filter a stream by explicitly giving the type arguments
			 * @tparam T being the value type
			 * @param pred being the predicate to use for filtering
			 * @return a filter toolbox tag for pipe operator
			 */
			template <class T>
			details::filter_toolbox<T> filter_(typename details::filter_toolbox<T>::pred_t pred){
				return {pred};
			}

			/**
			 * Pipe operator overload for filter operations
			 * @tparam T being the value type
			 * @param lhs being the stream to filter
			 * @param rhs being the filter toolbox tag
			 * @return The filtered stream
			 */
			template <class T>
			Stream<T> operator|(const Stream<T>& lhs, const details::filter_toolbox<T>& rhs){
				using namespace scl::tools;
				return Stream<T>{
					make::ptr<details::FilterOperator<T>>(lhs.it(), rhs.pred)
				};
			}
		}
	}
}