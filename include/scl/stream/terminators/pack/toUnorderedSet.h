#pragma once
#include <scl/stream/details/deprecation.h>
#include <unordered_set>
#include <scl/stream/Stream.h>
#include <scl/stream/details/iterator/EndStreamIterator.h>

namespace scl {
    namespace stream {
        namespace terminators {
            namespace pack {
                namespace details {
					/**
					 * Class that allows packing to a std::unordered_set
					 * @tparam T being the stream value type
					 * @tparam Hash being the type of hash function
					 * @tparam KeyEqual being the type of the key comparator
					 * @tparam Allocator being the type of the allocator used for std::unordered_set
					 */
                    template <class T, class Hash == std::hash<T>, class KeyEqual = std::equal_to<T>, class Allocator = std::allocator<T>>
                    class UnorderedSetPacker : public scl::stream::details::iterator::EndStreamIterator<std::unordered_set<T, Hash, KeyEqual, Allocator>, T>{
                        public:
                            using iterator_type = stream::details::iterator::EndStreamIterator<std::unordered_set<T, Hash, KeyEqual, Allocator>, T>;
                            using value_type = typename iterator_type::value_type;
                            using payload_type = typename iterator_type::payload_type;
                            using result_type = typename iterator_type::result_type;
                            using parent_iterator_type = typename iterator_type::parent_iterator_type;
                            using parent_type = typename iterator_type::parent_type;

                            using hash_type = Hash;
                            using key_equal_type = KeyEqual;
                            using allocator_type = Allocator;

                            explicit UnorderedSetPacker(parent_type p) : iterator_type{std::move(p)} {};

                            result_type process(){
                            	result_type ret;

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
					struct to_unordered_set_toolbox{};

					/**
					 * Tag type to pack into a std::set w/ control over template parameters
					 * @tparam Hash being the type of the hash function
					 * @tparam KeyEqual being the type of the key comparator
					 * @tparam Allocator being the type of allocator used to allocate memory
					 */
					template <class Hash, class KeyEqual, class Allocator>
					struct to_unordered_set_toolbox_alloc{};
                }

                details::to_unordered_set_toolbox toUnorderedSet(){ return {}; }

				/**
				 * Tag type to pack into a std::set w/ control over template parameters
				 * @tparam Hash being the type of the hash function
				 * @tparam KeyEqual being the type of the key comparator
				 * @tparam Allocator being the type of allocator used to allocate memory
				 */
				template <class Hash, class KeyEqual, class Allocator>
				details::to_unordered_set_toolbox_alloc<Hash, KeyEqual, Allocator> toUnorderedSet(){
					return {};
				}
            }

			/**
			 * Pipe operator for packing into an unordered set
			 * @tparam T being the value type of the stream
			 * @param lhs being the stream to pack
			 * @param _ being the packer type tag
			 * @return an unordered set containing the elements from the stream
			 */
			template <class T>
			typename pack::details::UnorderedSetPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_unordered_set_toolbox& _){
				auto packer = pack::details::UnorderedSetPacker<T>{lhs.it()};
				return packer.process();
			}

			/**
			 * Pipe operator or packing into an unordered set using a specific allocator and comparator
			 * @tparam T being the value type of the stream
			 * @tparam Hash being the type of the hash function
			 * @tparam KeyEqual being the type of the key comparator
			 * @tparam Allocator being the type of allocator used to allocate memory
			 * @param lhs being the stream to pack
			 * @param _ being the packer type tag
			 * @return an unordered set containing the elements from the stream
			 */
			template <class T, class Compare, class Allocator>
			typename pack::details::UnorderedSetPacker<T, Hash, KeyEqual, Allocator>::result_type operator|(const Stream<T>& lhs, const pack::details::to_unordered_set_toolbox_alloc<Hash, KeyEqual, Allocator>& _){
				auto packer = pack::details::UnorderedSetPacker<T, Hash, KeyEqual, Allocator>{lhs.it()};
				return packer.process();
			}
        }
    }
}