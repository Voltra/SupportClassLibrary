#pragma once

#include <scl/stream/details/iterator/EndStreamIterator.h>
#include <functional>
#include <scl/stream/Stream.h>
#include <scl/tools/meta/meta.hpp>

namespace scl{
	namespace stream{
		namespace terminators{
			namespace details{
				/**
				 * For each terminator
				 * @tparam T being the value type
				 */
				template <class T>
				class ForEachTerminator : public scl::stream::details::iterator::EndStreamIterator<void, T>{
					public:
						using iterator_type = scl::stream::details::iterator::EndStreamIterator<void, T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;
						using result_type = typename iterator_type::result_type;
						using parent_iterator_type = typename iterator_type::parent_iterator_type;
						using parent_type = typename iterator_type::parent_type;

						/**
						 * @typedef consumer_type
						 * The type of functions to call on each element
						 */
						using consumer_type = std::function<void(const value_type&)>;

						/**
						 * Construct a terminator from its parent and a consumer
						 * @param p being the parent iterator
						 * @param c being the consumer function
						 */
						ForEachTerminator(parent_type p, consumer_type c) : iterator_type{std::move(p)}, consumer{c} {
						}

						void process(){
							while(this->hasNext()){
								auto v = this->next();
								if(v.isValid())
									v.value().ifSome(consumer);
							}
							/*for(auto payload : *this) {
								if(payload.isValid())
									payload.value().doIfPresent(consumer);
							}*/
						};

					protected:
						/**
						 * @var consumer
						 * the function to call on each element
						 */
						consumer_type consumer;
				};


				/**
				 * forEach toolbox tag
				 * @tparam T being the value type
				 */
				template <class T>
				struct for_each_toolbox{
					/**
					 * @typedef callback_t
					 * The function type of a callback
					 */
					using callback_t = typename ForEachTerminator<T>::consumer_type;

					/**
					 * @var callback
					 * the callback
					 */
					callback_t callback;
				};
			}

			/**
			 * Invoke a function on each element of a stream
			 * @tparam F being the callback type (deduced)
			 * @tparam Fn being the function wrapper type (computed)
			 * @tparam T being the value type (computed)
			 * @param callback being the callback
			 * @return a toolbox tag for pipe operator
			 */
			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::remove_cv_ref_t<META::arg_t<Fn, 0>>
			>
			details::for_each_toolbox<T> forEach(F callback){
				return {callback};
			}

			/**
			 * Invoke a function on each element of a stream by providing the type arguments
			 * @tparam T being the value type
			 * @param cb being the callback function
			 * @return a toolbox tag for pipe operator
			 */
			template <class T>
			details::for_each_toolbox<T> forEach_(typename details::for_each_toolbox<T>::callback_t cb){
				return {cb};
			}

			/**
			 * Pipe operator overload for forEach terminators
			 * @tparam T being the value type
			 * @param lhs being the stream to consume
			 * @param rhs being the toolbox tag
			 */
			template <class T>
			void operator|(const Stream<T>& lhs, const details::for_each_toolbox<T>& rhs){
				auto terminator = details::ForEachTerminator<T>{lhs.it(), rhs.callback};
				terminator.process();
			}
		}
	}
}