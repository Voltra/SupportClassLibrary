#pragma once

#include <set>
#include <scl/stream/Stream.h>
#include <scl/stream/details/iterator/EndStreamIterator.h>

namespace scl {
	namespace stream {
		namespace terminators {
			namespace pack {
				namespace details {
					/**
					 * Class that allows packing to a std::set
					 * @tparam T being the stream value type
					 * @tparam Compare being the type of comparator used to insert elements
					 * @tparam Allocator being the type of the allocator used for std::vector
					 */
					template <class T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
					class SetPacker : public scl::stream::details::iterator::EndStreamIterator<std::set<T, Compare, Allocator>, T>{
						public:
							using iterator_type = stream::details::iterator::EndStreamIterator<std::set<T, Compare, Allocator>, T>;
							using value_type = typename iterator_type::value_type;
							using payload_type = typename iterator_type::payload_type;
							using result_type = typename iterator_type::result_type;
							using parent_iterator_type = typename iterator_type::parent_iterator_type;

							/**
							 * @typedef compare_type
							 * The type of comparator
							 */
							using compare_type = Compare;

							/**
							 * @typedef allocator_type
							 * The type of allocator
							 */
							using allocator_type = Allocator;

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

					template <class Compare, class Allocator>
					struct to_set_toolbox_alloc{};
				}

				details::to_set_toolbox toSet(){ return {}; }

				template <class Compare, class Allocator>
				details::to_set_toolbox_alloc<Compare, Allocator> toSet(){ return {}; }
			}

			template <class T>
			typename pack::details::SetPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_set_toolbox& _){
				auto packer = pack::details::SetPacker<T>{lhs.it()};
				return packer.process();
			}

			template <class T, class Compare, class Allocator>
			typename pack::details::SetPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_set_toolbox_alloc<Compare, Allocator>& _){
				auto packer = pack::details::SetPacker<T>{lhs.it()};
				return packer.process();
			}
		}
	}
}