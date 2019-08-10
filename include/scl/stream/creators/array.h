#pragma once

#include <array>
#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/stream/details/payload.h>
#include <scl/tools/make/make.hpp>

namespace scl{
	namespace stream{
		namespace creators{
			namespace details{
				/**
				 * Creator for std::array
				 * @tparam T being the type of data
				 * @tparam N being the amount of data
				 */
				template <class T, size_t N>
				class FromArrayIterator : public scl::stream::details::iterator::BaseStreamIterator<T>{
					public:
						using iterator_type = scl::stream::details::iterator::BaseStreamIterator<T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;

					protected:
						/**
						 * @var arr
						 * The underlying array
						 */
						std::array<T, N> arr;

						/**
						 * @var index
						 * The next index to get data from
						 */
						size_t index = 0;

					public:
						/**
						 * Construct from a std::array
						 * @param arr being the array to construct from
						 */
						explicit FromArrayIterator(std::array<T, N> arr) : arr{std::move(arr)}{
						}

						bool hasNext() const override{ return this->index < N; }

						payload_type next() override{
							if(!this->hasNext())
								return payload_type::withoutValue();

							const auto& value = this->arr[this->index];
							this->index += 1;

							return payload_type::withValue(value);
						}

						/*FromArrayIterator<T, N> clone() const override{
							return FromArrayIterator{this->arr};
						}*/
				};
			}

			/**
			 * Create an scl::stream::Stream from a std::array
			 * @tparam T being the type of values
			 * @tparam N being the length of the array
			 * @param a being the array
			 * @return a stream that uses the array as its source of data
			 */
			template <class T, size_t N>
			Stream<T> streamFrom(std::array<T, N> a){
				using namespace scl::tools;
				return Stream<T>{
					make::ptr<details::FromArrayIterator<T, N>>(std::move(a))
				};
			}
		}
	}
}