#pragma once

#include <scl/stream/details/iterator/EndStreamIterator.h>
#include <functional>
#include <scl/stream/Stream.h>
#include <scl/tools/meta/meta.hpp>

namespace scl{
	namespace stream{
		namespace terminators{
			template <class T>
			class ForEachTerminator : public scl::stream::details::iterator::EndStreamIterator<void, T>{
				public:
					using iterator_type = scl::stream::details::iterator::EndStreamIterator<void, T>;
					using value_type = typename iterator_type::value_type;
					using payload_type = typename iterator_type::payload_type;
					using result_type = typename iterator_type::result_type;
					using parent_iterator_type = typename iterator_type::parent_iterator_type;

					using consumer_type = std::function<void(const value_type&)>;

					ForEachTerminator(parent_iterator_type& p, consumer_type c) : iterator_type{p}, consumer{c} {
					}

					result_type process(){
						while(this->hasNext()){
							auto value = this->next().value();
							value.doIfLeft(consumer);
						}

						return;
					};

				protected:
					consumer_type consumer;
			};

			namespace details{
				template <class T>
				struct for_each_toolbox{
					using callback_t = typename ForEachTerminator<T>::consumer_type;
					callback_t callback;
				};
			}

			template <
				class F,
				class Fn = decltype(META::as_fn(std::declval<F>())),
				class T = META::remove_cv_ref_t<META::arg_t<Fn, 0>>
			>
			details::for_each_toolbox<T> forEach(F callback){
				return {callback};
			}

			template <class T>
			details::for_each_toolbox<T> forEach_(typename details::for_each_toolbox<T>::callback_t cb){
				return {cb};
			}

			template <class T>
			void operator|(const Stream<T>& lhs, const details::for_each_toolbox<T>& toolbox){
				auto terminator = ForEachTerminator<T>{lhs.it(), toolbox.callback};
				terminator.process();
			}
		}
	}
}