#pragma once

#include <scl/macros.h>
#include <scl/tools/meta/meta.hpp>
#include <limits>
#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/tools/make/make.hpp>

namespace scl{
	namespace stream{
		namespace creators{
			namespace details{
				template <class T>
				class RangeCreator : public scl::stream::details::iterator::BaseStreamIterator<T>{
					public:
						using iterator_type = scl::stream::details::iterator::BaseStreamIterator<T>;
						using value_type = typename iterator_type::value_type;
						using payload_type = typename iterator_type::payload_type;

					protected:
						T from, to, step, cur;

					public:
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

			template <class T = int, class = META::enable_if_t<META::is_arithmetic<T>()>>
			Stream<T> range(T from, T to, T step = 1){
				namespace make = scl::tools::make;
				return Stream<T>{
					make::ptr<details::RangeCreator<T>>(from, to, step)
				};
			}

			template <class T = int, class = META::enable_if_t<META::is_arithmetic<T>()>>
			Stream<T> rangeTo(T to, T step = 1){
				return range<T>(0, to, step);
			}

			template <class T = int, class = META::enable_if_t<META::is_arithmetic<T>()>>
			Stream<T> rangeFrom(T from, T step = -1){
				return range<T>(from, 0, step);
			}

			template <class T = double, class = META::enable_if_t<
				META::is_arithmetic<T>()
				&& std::numeric_limits<T>::has_infinity
			>>
			Stream<T> infiniteRange(T from = 0, T step = 1){
				return range<double>(from, std::numeric_limits<T>::infinity(), step);
			}
		}
	}
}