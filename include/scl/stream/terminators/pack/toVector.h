#pragma once

#include <vector>
#include <scl/stream/Stream.h>
#include <scl/stream/details/iterator/EndStreamIterator.h>

namespace scl{
	namespace stream{
		namespace terminators {
			namespace pack {
				namespace details {
					template<class T>
					class VectorPacker : public stream::details::iterator::EndStreamIterator<std::vector<T>, T> {
						public:
							using iterator_type = stream::details::iterator::EndStreamIterator<std::vector<T>, T>;
							using value_type = typename iterator_type::value_type;
							using payload_type = typename iterator_type::payload_type;
							using result_type = typename iterator_type::result_type;
							using parent_iterator_type = typename iterator_type::parent_iterator_type;

							VectorPacker(parent_iterator_type& p) : iterator_type{p} {
							}

							result_type process() {
								result_type ret = {};

								while (this->hasNext()) {
									auto alt = this->next().value();
									if (alt.hasValue())
										ret.push_back(*alt);
								}

								return ret;
							}
					};

					struct to_vector_toolbox{};
				}

				details::to_vector_toolbox toVector(){ return {}; }
			}

			template<class T>
			typename pack::details::VectorPacker<T>::result_type operator|(const Stream<T>& lhs, const pack::details::to_vector_toolbox& _) {
				auto packer = pack::details::VectorPacker<T>{lhs.it()};
				return packer.process();
			}
		}
	}
}