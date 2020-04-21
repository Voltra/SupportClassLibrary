#pragma once
#include <scl/stream/details/deprecation.h>
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
				/**
				 * Unique by operation
				 * @tparam T being the value type
				 * @tparam U being the type to use as key
				 */
				template <class T, class U>
				class UniqueByOperator : public scl::stream::details::iterator::OpStreamIterator<T>{
					public:
						using iterator_type = scl::stream::details::iterator::OpStreamIterator<T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;

						using parent_iterator_type = typename iterator_type::parent_iterator_type;
						using parent_value_type = typename parent_iterator_type::value_type;
						using parent_payload_type = typename parent_iterator_type::payload_type;
						using parent_type = typename iterator_type::parent_type;

						/**
						 * @typedef key_type
						 * The type of keys (unique by key)
						 */
						using key_type = U;

						/**
						 * @typedef mapper_type
						 * The function type that computes a key from a value
						 */
						using mapper_type = std::function<key_type(const value_type&)>;

						/**
						 * Construct an operator from its parent iterator and the mapper function
						 * @param p being the parent iterator
						 * @param key being the key computation function
						 */
						UniqueByOperator(parent_type p, mapper_type key) : iterator_type{std::move(p)}, key{key} {
						}

						payload_type next() override{
							bool condition = true;

							while(condition && this->parent()->hasNext()){
								const auto& alt = this->parent()->next().value();
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
						/**
						 * @var key
						 * the function that computes a key from a value
						 */
						mapper_type key;

						/**
						 * @var tagged
						 * the state that keeps track of all encountered keys
						 */
						std::unordered_set<key_type> tagged;
				};

				/**
				 * Toolbox tag type for unique by operations
				 * @tparam T being the value type
				 * @tparam U being the key type
				 */
				template <class T, class U>
				struct uniqueBy_toolbox{
					/**
					 * @typedef map_t
					 * The key computation function type
					 */
					using map_t = typename UniqueByOperator<T, U>::mapper_type;

					/**
					 * @var mapper
					 * the key computation function
					 */
					map_t mapper;
				};
			}

			/**
			 * Remove duplicates from a stream
			 * @tparam F being the callable type (deduce)
			 * @tparam Fn being the function wrapper type (computed)
			 * @tparam T being the value type (computed)
			 * @tparam U being the key type (computed)
			 * @param mapper being the key computation function
			 * @return a toolbox tag for pipe operator
			 */
			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::decay_t<META::arg_t<Fn, 0>>,
				class U = META::decay_t<META::return_t<Fn>>
			>
			details::uniqueBy_toolbox<T, U> uniqueBy(F&& mapper){
				return {META::as_fn(mapper)};
			}

			/**
			 * Remove duplicates from a stream by explicitly providing the type arguments
			 * @tparam T being the value type
			 * @tparam U being the key type
			 * @param mapper being the key computation function
			 * @return a toolbox tag for pipe operator
			 */
			template <class T, class U>
			details::uniqueBy_toolbox<T, U> uniqueBy_(typename details::uniqueBy_toolbox<T, U>::map_t mapper){
				return {mapper};
			}

			/**
			 * Pipe operator overload for uniqueBy (and unique) operations
			 * @tparam T being the value type
			 * @tparam U being the key type
			 * @param lhs being the stream to remove duplicates from
			 * @param rhs being the unique toolbox tag
			 */
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