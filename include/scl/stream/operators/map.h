#pragma once

#include <scl/stream/details/iterator/OpStreamIterator.h>
#include <functional>
#include <memory>
#include <scl/tools/make/make.hpp>
#include <scl/tools/meta/meta.hpp>

namespace scl{
	namespace stream{
		namespace operators{
			template <class T, class U>
			class MapOperator : public scl::stream::details::iterator::OpStreamIterator<U, T>{
				public:
					using iterator_type = scl::stream::details::iterator::OpStreamIterator<U, T>;
					using value_type = typename iterator_type::value_type;
					using payload_type = typename iterator_type::payload_type;

					using parent_iterator_type = typename iterator_type::parent_iterator_type;
					using parent_value_type = typename parent_iterator_type::value_type;
					using parent_payload_type = typename parent_iterator_type::payload_type;

					using mapper_type = std::function<value_type(const parent_value_type&)>;

					MapOperator(parent_iterator_type& p, mapper_type mapper) : iterator_type{p}, mapper{mapper} {
					}

					payload_type next() override{
						auto alt = this->parent().next().value();
						return payload_type{[=]{ return alt.template mapTo<value_type>(this->mapper); }};
					}

				protected:
					mapper_type mapper;
			};

			namespace details{
				template <class T, class U>
				struct map_toolbox{
					using mapper_t = typename MapOperator<T, U>::mapper_type;
					mapper_t mapper;
				};
			}


			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::decay_t<META::arg_t<Fn, 0>>,
				class U = META::decay_t<META::return_t<Fn>>
			>
			details::map_toolbox<T, U> map(F mapper){
				return {META::as_fn(mapper)};
			}

			template<class T, class U = T>
			details::map_toolbox<T, U> map_(typename details::map_toolbox<T, U>::mapper_t mapper){
				return {mapper};
			}

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