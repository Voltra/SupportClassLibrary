#pragma once
#include <scl/stream/details/deprecation.h>
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
							using parent_type = typename iterator_type::parent_type;

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

							explicit SetPacker(parent_type p) : iterator_type{std::move(p)} {
							}

							result_type process(){
								result_type ret;

								/*while(this->hasNext()){
									auto alt = this->next().value();
									if(alt.hasValue())
										ret.insert(*alt);
								}*/

								for(auto&& payload : *this){
									if(payload.isValid())
										ret.insert(*payload.value());
								}

								return ret;
							}
					};

					/**
					 * Tag type to pack into a std::set w/ default template parameters
					 */
					struct to_set_toolbox{};

					/**
					 * Tag type to pack into a std::set w/ control over template parameters
					 * @tparam Compare being the type of the comparator used to order the set
					 * @tparam Allocator being the type of allocator used to allocate memory
					 */
					template <class Compare, class Allocator>
					struct to_set_toolbox_alloc{};
				}

				/**
				 * Terminator for packing into a set
				 * @return a toolbox tag for pipe operator
				 */
				details::to_set_toolbox toSet(){ return {}; }

				/**
				 * Terminator for packing into a set w/ a specific comparator and allocator
				 * @tparam Compare being the comparator type
				 * @tparam Allocator being the allocator type
				 * @return a toolbox tag for pipe operator
				 */
				template <class Compare, class Allocator>
				details::to_set_toolbox_alloc<Compare, Allocator> toSet(){ return {}; }
			}

			/**
			 * Pipe operator for packing into a set
			 * @tparam T being the value type of the stream
			 * @param lhs being the stream to pack
			 * @param _ being the packer type tag
			 * @return a set containing the elements from the stream
			 */
			template <class T>
			typename pack::details::SetPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_set_toolbox& _){
				auto packer = pack::details::SetPacker<T>{lhs.it()};
				return packer.process();
			}

			/**
			 * Pipe operator or packing into a set using a specific allocator and comparator
			 * @tparam T being the value type of the stream
			 * @tparam Compare being the comparator type
			 * @tparam Allocator being the type of allocator to use
			 * @param lhs being the stream to pack
			 * @param _ being the packer type tag
			 * @return a vector containing the elements from the stream
			 */
			template <class T, class Compare, class Allocator>
			typename pack::details::SetPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_set_toolbox_alloc<Compare, Allocator>& _){
				auto packer = pack::details::SetPacker<T>{lhs.it()};
				return packer.process();
			}
		}
	}
}