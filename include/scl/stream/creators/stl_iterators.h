#pragma once

#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/macros.h>
#include <iterator>
#include <scl/tools/make/make.hpp>

namespace scl{
	namespace stream{
		namespace creators{
			namespace details{
				/**
				 * @typedef iterator_value_type
				 * A type alias for the value type from the iterator traits
				 */
				template <class It>
				using iterator_value_type = typename std::iterator_traits<It>::value_type;

				/**
				 * Creator for an iterator-based range
				 * @tparam It being the type of the iterators
				 */
				template <class It>
				class FromStlIterator : public scl::stream::details::iterator::BaseStreamIterator<iterator_value_type<It>>{
					public:
						using value_type = iterator_value_type<It>;
						using iterator_type = scl::stream::details::iterator::BaseStreamIterator<value_type>;
						using payload_type = typename iterator_type::payload_type;

					protected:
						/**
						 * @var begin
						 * the iterator to the first element
						 *
						 * @var end
						 * the iterator to the point after the last element
						 */
						It begin, end;

					public:
						/**
						 * Construct the iterator from STL-like iterators
						 * @param begin being the beginning of the range
						 * @param end being the end of the range
						 */
						FromStlIterator(It begin, It end) : begin{begin}, end{end} {
						}

						bool hasNext() const override{ return begin != end; }

						payload_type next() override{
							if(!this->hasNext())
								return payload_type::withoutValue();

							const value_type& value = *(begin++);
							return payload_type::withValue(value);
						}

						/*FromStlIterator<It> clone() override{}*/
				};
			}

			/**
			 * Create a stream from a pair of iterators
			 * @tparam It being the type of iterators
			 * @param begin being the begin iterator
			 * @param end being the end iterator
			 * @return a stream that emits the range's values
			 */
			template <class It>
			Stream<typename details::FromStlIterator<It>::value_type> streamFrom(It begin, It end){
				using namespace scl::tools;
				return Stream<typename details::FromStlIterator<It>::value_type>{
					make::ptr<details::FromStlIterator<It>>(begin, end)
				};
			}
		}
	}
}