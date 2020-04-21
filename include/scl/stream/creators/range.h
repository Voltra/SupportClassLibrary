#pragma once
#include <scl/stream/details/deprecation.h>
#include <scl/macros.h>
#include <scl/tools/meta/meta.hpp>
#include <limits>
#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/tools/make/make.hpp>
#include <scl/concepts/require.h>
#include <scl/concepts/Copyable.h>

namespace scl{
	namespace stream{
		namespace creators{
			namespace details{
				/**
				 * Creator for a numeric range
				 * @tparam T being the numeric type used for the range
				 */
				template <class T>
				class RangeCreator : public scl::stream::details::iterator::BaseStreamIterator<T>{
						static_require(concepts::Copyable<T>{});

					public:
						using iterator_type = scl::stream::details::iterator::BaseStreamIterator<T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;

					protected:
						/**
						 * @var from
						 * the starting point
						 *
						 * @var to
						 * the final point
						 *
						 * @var step
						 * the individual increment
						 *
						 * @var cur
						 * the current tate
						 */
						T from, to, step, cur;

					public:
						/**
						 * Construct from initial, last and increment
						 * @param from being the initial value
						 * @param to being the last value
						 * @param step being the increment value
						 */
						RangeCreator(T from, T to, T step) : from{from}, to{to}, step{step}, cur{from} {
						}

						bool hasNext() const override{
							return this->step > 0 ? this->cur < this->to : this->cur >= this->to;
						}

						payload_type next() override{
							if(!this->hasNext())
								return payload_type::withoutValue();

							T value = this->cur;
							this->cur += this->step;
							return payload_type::withValue(value);
						}
				};
			}

			/**
			 * Create an scl::stream::Stream for a specified range
			 * @tparam T being the type of the elements of the range
			 * @param from being the initial value
			 * @param to being the last value
			 * @param step being the increment
			 * @return a stream that generates the range's values
			 */
			template <class T = int, class = META::enable_if_t<META::is_arithmetic<T>()>>
			Stream<T> range(T from, T to, T step = 1){
				namespace make = scl::tools::make;
				return Stream<T>{
					make::ptr<details::RangeCreator<T>>(from, to, step)
				};
			}

			/**
			 * Range from 0 to the specified value
			 * @tparam T being the type of the elements of the range
			 * @param to being the last value
			 * @param step being the increment
			 * @return a stream that generates the range's values
			 */
			template <class T = int, class = META::enable_if_t<META::is_arithmetic<T>()>>
			Stream<T> rangeTo(T to, T step = 1){
				return range<T>(0, to, step);
			}

			/**
			 * Generate a range from the given value to 0
			 * @tparam T being the type of the elements of the range
			 * @param from being the initial value
			 * @param step being the increment
			 * @return a stream that generates the range's values
			 */
			template <class T = int, class = META::enable_if_t<META::is_arithmetic<T>()>>
			Stream<T> rangeFrom(T from, T step = -1){
				return range<T>(from, 0, step);
			}

			/**
			 * Generate a range from the given value to +inf
			 * @tparam T being the type of the elements of the range
			 * @param from being the initial value
			 * @param step being the increment
			 * @return a stream that generates the range's values
			 */
			template <class T = double, class = META::enable_if_t<
				META::is_arithmetic<T>()
				&& std::numeric_limits<T>::has_infinity
			>>
			Stream<T> infiniteRange(T from = 0, T step = 1){
				return range<T>(from, std::numeric_limits<T>::infinity(), step);
			}
		}
	}
}