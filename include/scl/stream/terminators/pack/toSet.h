#pragma once

#include <set>
#include <scl/stream/Stream.h>
#include <scl/stream/details/iterator/EndStreamIterator.h>

namespace scl {
	namespace stream {
		namespace terminators {
			namespace pack {
				namespace details {
					template <class T>
					class SetPacker : public scl::stream::details::iterator::EndStreamIterator<std::set<T>, T>{
						public:
							using iterator_type = stream::details::iterator::EndStreamIterator<std::set<T>, T>;
							using value_type = typename iterator_type::value_type;
							using payload_type = typename iterator_type::payload_type;
							using result_type = typename iterator_type::result_type;
							using parent_iterator_type = typename iterator_type::parent_iterator_type;

							SetPacker(parent_iterator_type& p) : iterator_type{p} {
							}

							result_type process(){
								result_type ret;

								while(this->hasNext()){
									auto alt = this->next().value();
									if(alt.hasValue())
										ret.insert(*alt);
								}

								return ret;
							}
					};

					struct to_set_toolbox{};
				}

				details::to_set_toolbox toSet(){ return {}; }
			}

			template <class T>
			typename pack::details::SetPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_set_toolbox& _){
				auto packer = pack::details::SetPacker<T>{lhs.it()};
				return packer.process();
			}
		}
	}
}