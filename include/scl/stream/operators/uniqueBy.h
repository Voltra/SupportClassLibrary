#pragma once
#include <scl/stream/details/iterator/OpStreamIterator.h>
#include <functional>
#include <memory>
#include <scl/tools/make/make.hpp>
#include <scl/tools/meta/meta.hpp>
#include <unordered_set>
#include <scl/stream/Stream.h>

namespace scl{
	namespace stream{
		namespace operators{
			namespace details{
				template <class T, class U>
				class UniqueByOperator : public scl::stream::details::iterator::OpStreamIterator<T>{
					public:
						using iterator_type = scl::stream::details::iterator::OpStreamIterator<T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;

						using parent_iterator_type = typename iterator_type::parent_iterator_type;
						using parent_value_type = typename parent_iterator_type::value_type;
						using parent_payload_type = typename parent_iterator_type::payload_type;

						using key_type = U;
						using mapper_type = std::function<key_type(const value_type&)>;

						UniqueByOperator(parent_iterator_type& p, mapper_type key) : iterator_type{p}, key{key} {
						}

						payload_type next() override{
							bool condition = true;

							while(condition && this->parent().hasNext()){
								const auto& alt = this->parent().next().value();
								if(!alt.hasValue())
									continue;

								const auto& value = *alt;
								auto id = this->key(value);
								condition = !this->tagged.count(id); //count(id) == 0 => not found
								//!count(id) => true if doesn't has value

								if(condition){
									this->tagged.insert(id);
									return payload_type::withValue(value);
								}
							}

							return payload_type::withoutValue();
						}

					protected:
						mapper_type key;
						std::unordered_set<key_type> tagged;
				};

				template <class T, class U>
				struct uniqueBy_toolbox{
					using map_t = typename UniqueByOperator<T, U>::mapper_type;
					map_t mapper;
				};
			}

			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::decay_t<META::arg_t<Fn, 0>>,
				class U = META::decay_t<META::return_t<Fn>>
			>
			details::uniqueBy_toolbox<T, U> uniqueBy(F&& mapper){
				return {META::as_fn(mapper)};
			}

			template <class T, class U>
			details::uniqueBy_toolbox<T, U> uniqueBy(typename details::uniqueBy_toolbox<T, U>::map_t mapper){
				return {mapper};
			}

			template <class T, class U>
			Stream<T> operator|(const Stream<T>& lhs, details::uniqueBy_toolbox<T, U> rhs){
				using namespace scl::tools;
				return Stream<T>{
					make::ptr<details::UniqueByOperator<T, U>>(lhs.it(), rhs.mapper)
				};
			}
		}
	}
}