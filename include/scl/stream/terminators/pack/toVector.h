#pragma once

#include <vector>
#include <scl/stream/Stream.h>
#include <scl/stream/details/iterator/EndStreamIterator.h>

namespace scl{
	namespace stream{
		namespace terminators {
			namespace pack {
				namespace details {
					/**
					 * Class that allows packing to a std::vector
					 * @tparam T being the stream value type
					 * @tparam Allocator being the type of the allocator used for std::vector
					 */
					template<class T, class Allocator = std::allocator<T>>
					class VectorPacker : public stream::details::iterator::EndStreamIterator<std::vector<T, Allocator>, T> {
						public:
							using iterator_type = stream::details::iterator::EndStreamIterator<std::vector<T, Allocator>, T>;
							using value_type = typename iterator_type::value_type;
							using payload_type = typename iterator_type::payload_type;
							using result_type = typename iterator_type::result_type;
							using parent_iterator_type = typename iterator_type::parent_iterator_type;

							/**
							 * @typedef allocator_type
							 * The type of allocator used w/ std::vector
							 */
							using allocator_type = Allocator;

							explicit VectorPacker(parent_iterator_type& p) : iterator_type{p} {
							}

							result_type process() {
								result_type ret = {};

								/*while (this->hasNext()) {
									const auto& alt = this->next().value();
									if (alt.hasValue())
										ret.push_back(*alt);
								}*/

								for(auto&& payload : *this){
									if(payload.isValid())
										ret.push_back(*payload.value());
								}

								return ret;
							}
					};

					/**
					 * Tag type to pack into a vector w/ default template parameters
					 */
					struct to_vector_toolbox{};

					/**
					 * Tag type to pack into a vector w/ control over the allocator
					 * @tparam Allocator being the allocator type
					 */
					template <class Allocator>
					struct to_vector_toolbox_alloc{};
				}

				/**
				 * Terminator that packs into a vector
				 * @return the appropriate type tag
				 */
				details::to_vector_toolbox toVector(){ return {}; }

				/**
				 * Terminator that packs into a vector w/ a specific allocator
				 * @return the appropriate type tag
				 */
				template <class Allocator>
				details::to_vector_toolbox_alloc<Allocator> toVector(){ return {}; }
			}

			/**
			 * Pipe operator for packing into a vector
			 * @tparam T being the value type of the stream
			 * @param lhs being the stream to pack
			 * @param _ being the packer type tag
			 * @return a vector containing the elements from the stream
			 */
			template<class T>
			typename pack::details::VectorPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_vector_toolbox& _) {
				auto packer = pack::details::VectorPacker<T>{lhs.it()};
				return packer.process();
			}

			/**
			 * Pipe operator or packing into a vector using a specific allocator
			 * @tparam T being the value type of the stream
			 * @tparam Allocator being the type of allocator to use
			 * @param lhs being the stream to pack
			 * @param _ being the packer type tag
			 * @return a vector containing the elements from the stream
			 */
			template<class T, class Allocator>
			typename pack::details::VectorPacker<T, Allocator>::result_type operator|(const Stream<T>& lhs, const pack::details::to_vector_toolbox_alloc<Allocator>& _) {
				auto packer = pack::details::VectorPacker<T, Allocator>{lhs.it()};
				return packer.process();
			}
		}
	}
}