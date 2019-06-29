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
			template <class T>
			class FilterOperator : public scl::stream::details::iterator::OpStreamIterator<T>{
				public:
					using iterator_type = scl::stream::details::iterator::OpStreamIterator<T>;
					using value_type = typename iterator_type::value_type;
					using payload_type = typename iterator_type::payload_type;
					using parent_iterator_type = typename iterator_type::parent_iterator_type;

					using predicate_type = std::function<bool(const value_type&)>;

					FilterOperator(parent_iterator_type& p, predicate_type pred) : iterator_type{p}, pred{pred} {
					}

					payload_type next() override{
						while(this->parent().hasNext()){
							const auto& alt = this->parent().next().value();
							if(alt.hasValue()){
								const auto& value = *alt;
								if(this->pred(value))
									return payload_type::withValue(value);
							}
						}

						return payload_type::withoutValue();
					}
				protected:
					predicate_type pred;
			};

			namespace details{
				template <class T>
				struct filter_toolbox{
					using pred_t = typename FilterOperator<T>::predicate_type;
					pred_t pred;
				};
			}

			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::decay_t<META::arg_t<Fn, 0>>
			>
			details::filter_toolbox<T> filter(F&& predicate){
				return {META::as_fn(predicate)};
			}

			template <class T>
			details::filter_toolbox<T> filter_(typename details::filter_toolbox<T>::pred_t pred){
				return {pred};
			}

			template <class T>
			Stream<T> operator|(const Stream<T>& lhs, const details::filter_toolbox<T>& rhs){
				using namespace scl::tools;
				return Stream<T>{
					make::ptr<FilterOperator<T>>(lhs.it(), rhs.pred)
				};
			}
		}
	}
}